#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>

static void adc_init(void);
static void gpio_init(void);
static uint16_t adc_get(void);

static uint16_t adcval;

int main(void)
{
	system_init();
	gpio_init();
	adc_init();

	while (1)
	{
		adcval = adc_get();
		adcval = (adcval * 3000) / 4096;
		printf("adcval=%dmV\r\n", adcval);
		delay_ms(1000);
	}
}

static uint16_t adc_get(void)
{
	adc_start_conversion_direct(ADC1);

	/* Wait for end of conversion. */
	while (!(adc_eoc(ADC1)));

	return adc_read_regular(ADC1);
}

static void adc_init(void)
{
	uint8_t read_channel = 1;
	rcc_periph_clock_enable(RCC_ADC1);

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

static void gpio_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT,		
				  GPIO_CNF_INPUT_ANALOG, GPIO1);		
}
