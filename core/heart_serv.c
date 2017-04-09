#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/adc.h"
#include "libopencm3/stm32/timer.h"
#include "libopencm3/cm3/nvic.h"
#include "core/periodic_task.h"
#include "core/graphics.h"
#include "core/serv_core.h"
#include "core/ili9163.h"

/*---------------------------------------------------------------------------*/
typedef enum
{
	TRUE = 1,
	FALSE = 0
} flag_t;

struct heart_rate_params
{
	uint16_t thresh;
	uint16_t peak;
	uint16_t trough;
	uint16_t amplitude;

	uint16_t period;
	uint16_t time_count;
	uint16_t last_beat_time;

	uint16_t ibi; /* inter beat interval */
	uint16_t bpm;  /* beate per min */
	
	flag_t is_pulse;
	flag_t is_beat;

	flag_t is_first_beat;
	flag_t is_second_beat;
};

/*---------------------------------------------------------------------------*/
static uint16_t _adc_get(void);
static void _adc_init(void);
static void _heart_rate_sample(void);
static void (*heart_event)(uint16_t bpm, uint16_t *ibi, uint8_t ibilen);

/*---------------------------------------------------------------------------*/
static const struct font mono7x13 = 
{
	.width = 7,
	.height = 13,
	.data = DejaVu_Sans_Mono7x13,
	.text_color = WHITE,
	.bkg_color = BLACK
};

static struct font cali18x20= 
{
	.width = 18,
	.height = 20,
	.data = Calibri18x20,
	.text_color = WHITE,
	.bkg_color = BLACK
};

static uint16_t ibi_rate[10];

static struct heart_rate_params params = 
{
	.thresh = 1200,
	.peak = 1200,
	.trough = 1200,
	.amplitude = 100,

	.period = 5000,
	.time_count = 0,
	.last_beat_time = 0,

	.ibi = 600,
	.bpm = 0,
	
	.is_pulse = FALSE,
	.is_beat = FALSE,

	.is_first_beat = TRUE,
	.is_second_beat = TRUE,
};

uint8_t bpm_x = 0;

/*---------------------------------------------------------------------------*/
void heart_serv_init(void)
{
	uint16_t str_width = 0;

    str_width = font_get_str_width(&cali18x20, "000");
	bpm_x = (lcd_conf.lcd_x_size - str_width) / 2;

	ili9163_set_screen(&lcd_conf, BLACK);
	ili9163_draw_pic(&lcd_conf, lcd_conf.lcd_x_size/2 - 16 , 0, 32, 32, heartcolor32x32);

	_adc_init();
	periodic_task_register(_heart_rate_sample, 2, TASK_INTERRUPT_CONTEX);
}

void heart_serv_deinit(void)
{
	periodic_task_unregister(_heart_rate_sample);
}

void heart_serv_add_event(void (*event)(uint16_t, uint16_t *, uint8_t))
{
	heart_event = event;
}

void heart_serv_update(void)
{
	char buff[30];
	
	if (params.bpm > 60)
	{
		sprintf(buff, "%.3d", params.bpm);
		ili9163_print(&lcd_conf, bpm_x, 32 + 5, buff, &cali18x20);

		ili9163_print(&lcd_conf, 0, 32 + 5 + cali18x20.height + 5, "last 5 ibi", &mono7x13);

		sprintf(buff, "%.3d %.3d %.3d", ibi_rate[0], ibi_rate[1], ibi_rate[2]);
		ili9163_print(&lcd_conf, 0, 32 + 5 + cali18x20.height + 5 + mono7x13.height, buff, &mono7x13);

		sprintf(buff, "%.3d %.3d %.3d", ibi_rate[3], ibi_rate[4], ibi_rate[5]);
		ili9163_print(&lcd_conf, 0, 32 + 5 + cali18x20.height + 5 + mono7x13.height * 2, buff, &mono7x13);
	}
	else
	{
		ili9163_set_screen(&lcd_conf, BLACK);
		ili9163_draw_pic(&lcd_conf, lcd_conf.lcd_x_size/2 - 16 , 0, 32, 32, heartcolor32x32);
	}
}
/*---------------------------------------------------------------------------*/
static void 
_heart_rate_get_peak_trough(uint16_t adcval, uint16_t delta)
{
	if ((adcval < params.thresh) 
		&& (delta > ((params.ibi/5) * 3)))
	{
		if (adcval < params.trough)
			params.trough = adcval;
	}

	if ((adcval > params.thresh) && (adcval > params.peak))
	{
		params.peak = adcval;
	}
}

static void 
_heart_rate_sample(void)
{
	uint16_t adcval = 0;
	uint16_t delta = 0;

	adcval = _adc_get();
	params.time_count += 2;
	params.period -= 1;
	delta = params.time_count - params.last_beat_time;

	_heart_rate_get_peak_trough(adcval, delta);

	if (delta > 250)
	{
		uint8_t i = 0;
		uint16_t running_total = 0;

		if ((adcval > params.thresh) && (FALSE == params.is_pulse)
			&& (delta > ((params.ibi/5) * 3)))
		{
			params.is_pulse = TRUE;
			led_b2_on();
			params.ibi = params.time_count - params.last_beat_time;
			params.last_beat_time = params.time_count;

			if (TRUE == params.is_first_beat)
			{
				params.is_first_beat = FALSE;
				return;
			}
			if (TRUE == params.is_second_beat)
			{
				params.is_second_beat = FALSE;
				for (i = 0; i <= 9; i++)
				{
					ibi_rate[i] = params.ibi;
				}
			}
		for (i = 0; i <= 8; i++)
		{
			ibi_rate[i] = ibi_rate[i + 1];
			running_total += ibi_rate[i];
		}

			ibi_rate[9] = params.ibi;
			running_total += ibi_rate[9];
			running_total /= 10;
			params.bpm = 60000 / running_total;
			params.is_beat = TRUE;
		
		}
	}

	if ((adcval < params.thresh) && (TRUE == params.is_pulse))
	{
		led_b2_off();
		params.is_pulse = FALSE;
		params.amplitude = params.peak - params.trough;
		params.thresh = params.amplitude / 2 + params.trough;
		params.peak = params.thresh;
		params.trough = params.thresh;
	}

	if (delta > 2500)
	{
		params.thresh = 1200;
		params.peak = 1200;
		params.trough = 1200;
		params.last_beat_time = params.time_count;
		params.is_first_beat = TRUE;
		params.is_second_beat = TRUE;
		params.bpm = 0;
	}

	if (0 == params.period)
	{
		if (NULL != heart_event)
		{
			heart_event(params.bpm, ibi_rate, 10);
		}
		params.period = 5000;     	
	}
}

static uint16_t
_adc_get(void)
{
	adc_start_conversion_direct(ADC1);
	while (!(adc_eoc(ADC1)));
	return adc_read_regular(ADC1);
}

static void 
_adc_init(void)
{
	uint8_t read_channel = 1;

	/* Make sure the ADC doesn't run during config. */
	adc_power_off(ADC1);

	/* We configure everything for one single conversion. */
	adc_disable_scan_mode(ADC1);
	adc_set_single_conversion_mode(ADC1);
	adc_disable_external_trigger_regular(ADC1);
	adc_set_right_aligned(ADC1);
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_28DOT5CYC);

	adc_power_on(ADC1);

	/* Wait for ADC starting up. */
	delay_ms(100);

	adc_reset_calibration(ADC1);
	adc_calibrate(ADC1);
	adc_set_regular_sequence(ADC1, 1, &read_channel);
}


