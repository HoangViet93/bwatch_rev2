#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include "core/clock_rtc.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/rtc.h"

struct time t;
struct date d;

volatile uint32_t elapsed_sec = 3000 * 24 * 3600;

int main(void)
{
	system_init();
	
	clock_rtc_init(RCC_LSI);
	
	d.day = 11;
	d.month = 3;
	d.year = 2017;
	
	printf("set date to => day = %d, month = %d, year = %d\r\n", 
           d.day, d.month, d.year);

	clock_rtc_set_date(d);
	rtc_set_counter_val(elapsed_sec);
	clock_rtc_get_date(&d);

	printf("after %d hour \r\n", (int)(elapsed_sec / 3600));
	printf("day = %d, month = %d, year = %d\r\n", d.day, d.month, d.year);

	while (1)
	{
		led_b2_toggle();
		delay_ms(1000);
	}
}


