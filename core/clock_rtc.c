#include "clock_rtc.h"

void
clock_rtc_init(void)
{
	rcc_osc_on(RCC_LSI);
	rcc_wait_for_osc_ready(RCC_LSI);
	rtc_auto_awake(RCC_LSI, 40000);

	nvic_enable_irq(NVIC_RTC_IRQ);
	nvic_set_priority(NVIC_RTC_IRQ, 1);
	rtc_interrupt_enable(RTC_SEC);
}
