#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/i2c.h"
#include "libopencm3/cm3/nvic.h"
#include "libopencm3/stm32/exti.h"
#include "core/adxl345.h"
#include "core/graphics.h"
#include "core/activity_serv.h"
#include "core/ili9163.h"
#include "core/serv_core.h"
#include "core/periodic_task.h"


/* debug purpose */
#if (1) || defined(CONFIG_ENABLE_DEBUG) 
#include "stdio.h"
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#define ACTIVITY_INT_PRIORITY	(7 * 16)

static void _activity_serv_init_gyro(void);
static void _activity_serv_init_exti(void);
static void _calculate_position_params(void);
static int16_t _activity_read_event(void);


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

static void (*event_pfunc)(uint8_t event) = NULL;
static uint16_t act_x, act_y;

void 
activity_serv_init(void)
{
	_calculate_position_params();

	LCD_LOCK();
    ili9163_set_screen(&lcd_conf, BLACK);
    LCD_UNLOCK();

	_activity_serv_init_exti();
	_activity_serv_init_gyro();
}

static void
_calculate_position_params(void)
{   
	uint16_t str_width = 0;
    uint16_t str_height = 0;
	
    /* pre-calculate position of digital clock */
    str_width = font_get_str_width(&cali18x20, "inactivity");
    str_height = font_get_str_height(lcd_conf.lcd_x_size, &cali18x20, "inactivity");

    act_y = lcd_conf.lcd_y_size/2 - str_height/2;
    act_x = lcd_conf.lcd_x_size/2 - str_width/2;
}

int8_t
activity_serv_event_cb_add(void (*pfunc)(uint8_t event))
{
	if (NULL != event_pfunc)
	{
		return -1;
	}
	else
	{
		event_pfunc = pfunc;
		return 0;
	}
}

int8_t
activity_serv_event_cb_remove(void (*pfunc)(uint8_t event))
{
	if (pfunc != event_pfunc)
	{
		return -1;
	}
	else
	{
		event_pfunc = NULL;
		return 0;
	}
}

void
activity_serv_event_enable(void)
{
    nvic_set_priority(NVIC_EXTI1_IRQ, ACTIVITY_INT_PRIORITY);
	nvic_enable_irq(NVIC_EXTI1_IRQ);
}

void
activity_serv_event_disable(void)
{
    nvic_disable_irq(NVIC_EXTI1_IRQ);
}

void 
activity_serv_axis_update(void)
{
	struct axis axis;
	float x,y,z = 0;
	char buff[15];
	
	adxl345_axis_read(&adxl345_conf, &axis);
	x = axis.x * 0.004 * 9.806;
	y = axis.y * 0.004 * 9.806;
	z = axis.z * 0.004 * 9.806;
	
	sprintf(buff, "x = %.2d  ", axis.x);
	ili9163_print(&lcd_conf, 0, 0, buff, &mono7x13);
}


static void
_free_fall_alarm(void)
{
	static uint8_t toggle = 0;
	static uint8_t count = 0;

	if (0 == toggle)
	{
		cali18x20.text_color = BLACK;
		toggle = 1;
	}
	else
	{
		cali18x20.text_color = RED;
		toggle = 0;
	}

	count++;

	ili9163_print(&lcd_conf, act_x, act_y, "free fall ", &cali18x20);

	if (20 == count)
	{
		count = 0;
	
		periodic_task_unregister(_free_fall_alarm);
		cali18x20.text_color = WHITE;
		cali18x20.bkg_color = BLACK;
		
		activity_serv_update_stt(ACT_EVENT_INACTIVITY);
		activity_serv_event_enable();
	}
}
	
void
activity_serv_update_stt(uint8_t event)
{
	uint16_t bkp_color = WHITE;
	static uint8_t ff_detect_step = 0;	

	bkp_color = cali18x20.text_color;
	switch (event)
	{
		case ACT_EVENT_ACTIVITY:
			printf("ACT_EVENT_ACTIVITY\r\n");
			if ((ff_detect_step & ADXL345_FREE_FALL) == ADXL345_FREE_FALL)
			{
				ff_detect_step |= ADXL345_ACTIVITY;
			}
			else
			{
				ff_detect_step = 0;
				cali18x20.text_color = GREEN;
				ili9163_print(&lcd_conf, act_x, act_y, "activity  ", &cali18x20);
			}
			break;
		case ACT_EVENT_INACTIVITY:
			printf("ACT_EVENT_INACTIVITY\r\n");
			if ((ff_detect_step & ADXL345_ACTIVITY) == ADXL345_ACTIVITY)
			{
				activity_serv_event_disable();
				periodic_task_register(_free_fall_alarm, 250 - 1, TASK_THREAD_CONTEX);
				ff_detect_step = 0;
			}
			else
			{
				ff_detect_step = 0;
				cali18x20.text_color = WHITE;
				ili9163_print(&lcd_conf, act_x, act_y, "inactivity", &cali18x20);
			}
			break;
		case ACT_EVENT_FF:
			printf("ACT_EVENT_FF\r\n");
			ff_detect_step |= ADXL345_FREE_FALL;
			break;
		default:
			break;
	}
	cali18x20.text_color = bkp_color;
}

void
activity_serv_deinit(void)
{
	event_pfunc = NULL;
	activity_serv_event_disable();
}

static void 
_activity_serv_init_gyro(void)
{
	__attribute__((unused)) uint8_t int_source = 0;

	adxl345_init(&adxl345_conf);
	
	adxl345_write_byte(adxl345_conf.i2c, ADXL345_POWER_CTL, 0x28);
	adxl345_range_set(&adxl345_conf, ADXL345_RANGE_16G);
	adxl345_write_byte(adxl345_conf.i2c, ADXL345_BW_RATE, ADXL345_RATE_100HZ);	

	adxl345_activity_thresh_set(&adxl345_conf, 2000);
	adxl345_activity_ctl(&adxl345_conf, ADXL345_ACT_X | ADXL345_ACT_Y | ADXL345_ACT_Z);

	adxl345_inactivity_thresh_set(&adxl345_conf, 185);
	adxl345_inactivity_ctl(&adxl345_conf, ADXL345_INACT_ACDC | ADXL345_INACT_X  \
				          | ADXL345_INACT_Y | ADXL345_INACT_Z);
	adxl345_write_byte(adxl345_conf.i2c, ADXL345_TIME_INACT, 2);	

	adxl345_freefall_thresh_set(&adxl345_conf, 750);
	adxl345_freefall_time_set(&adxl345_conf, 30);

	adxl345_interrupt_set(&adxl345_conf, INT_PIN_1, ADXL345_ACTIVITY | ADXL345_INACTIVITY
						  | ADXL345_FREE_FALL);
	
	int_source = adxl345_read_byte(adxl345_conf.i2c, ADXL345_INT_SOURCE);
	int_source = adxl345_read_byte(adxl345_conf.i2c, ADXL345_INT_SOURCE);
}

static void 
_activity_serv_init_exti(void)
{
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, 
                  GPIO_CNF_INPUT_FLOAT, GPIO1);
	gpio_clear(GPIOB, GPIO1);

	/* Configure the EXTI subsystem. */
	exti_select_source(EXTI1, GPIOB);
	exti_set_trigger(EXTI1, EXTI_TRIGGER_RISING);
	exti_enable_request(EXTI1);
}

static int16_t 
_activity_read_event(void)
{
	uint8_t int_src = 0;
	
	int_src =  adxl345_read_byte(adxl345_conf.i2c, ADXL345_INT_SOURCE);
	if ((int_src & ADXL345_ACTIVITY) == ADXL345_ACTIVITY)
	{
		return ACT_EVENT_ACTIVITY;
	}
	else if ((int_src & ADXL345_INACTIVITY) == ADXL345_INACTIVITY)
	{
		return ACT_EVENT_INACTIVITY;
	}
	else if ((int_src & ADXL345_FREE_FALL) == ADXL345_FREE_FALL)
	{
		return ACT_EVENT_FF;
	}
	else
	{
		return -1;
	}
}

void exti1_isr(void)
{
	uint8_t event = 0;

    if (exti_get_flag_status(EXTI1) != EXTI1)
    {
        return;
    }

	event = _activity_read_event();
	if ((event_pfunc != NULL) && (event > 0))
	{
		event_pfunc(event);
	}
	
    exti_reset_request(EXTI1);
}

