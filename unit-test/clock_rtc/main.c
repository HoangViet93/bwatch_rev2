#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include "core/clock_rtc.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/rtc.h"

static void rtc_cb(void);

struct time t;
struct date d;
volatile uint32_t sec_count = 0;

int main(void)
{
	system_init();
	
	clock_rtc_init(RCC_LSI);
	clock_rtc_add_cb(rtc_cb);

	clock_rtc_get_date(&d);
	printf("day = %d, month = %d, year = %d\r\n", d.day, d.month, d.year);

	clock_rtc_enable_interrupt(1);

	delay_ms(10000);
	clock_rtc_remove_cb(rtc_cb);

	while (1)
	{
		led_b2_toggle();
		delay_ms(1000);
	}
}

static void rtc_cb(void)
{
	clock_rtc_get_time(&t);
	printf("%d:%d:%d\r\n", t.hour, t.minutes, t.second);
	sec_count = rtc_get_counter_val();
	printf("sec_count=%ld\r\n", sec_count);
}
