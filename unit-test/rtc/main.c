#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/cm3/nvic.h>

void rtc_setup(void);

int main(void)
{
	system_init();
	
	rcc_osc_on(RCC_LSI);
	rcc_wait_for_osc_ready(RCC_LSI);

	rtc_setup();

	while (1)
	{
		led_b2_toggle();
		delay_ms(1000);
	}
}

void rtc_isr(void)
{
	volatile uint32_t sec_count = 0;

	rtc_clear_flag(RTC_SEC);
	
	sec_count = rtc_get_counter_val();
	printf("sec_count=%d\r\n", sec_count);
}

void rtc_setup(void)
{
	rtc_auto_awake(RCC_LSI, 40000);

	nvic_enable_irq(NVIC_RTC_IRQ);
	nvic_set_priority(NVIC_RTC_IRQ, 1);
	rtc_interrupt_enable(RTC_SEC);
}
