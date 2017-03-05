#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include "core/clock_rtc.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/rtc.h"

static uint8_t conv2d(const char* p);

struct time t;
volatile uint32_t sec_count = 0;

int main(void)
{
	system_init();
	
	clock_rtc_init(RCC_LSI);
	
	while (1)
	{
		clock_rtc_get_time(&t);
		printf("%d:%d:%d\r\n", t.hour, t.minutes, t.second);
		sec_count = rtc_get_counter_val();
		printf("sec_count=%ld\r\n", sec_count);
		led_b2_toggle();
		delay_ms(1000);
	}
}

