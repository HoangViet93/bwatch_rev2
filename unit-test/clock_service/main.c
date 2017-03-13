#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/spi.h"
#include "core/ili9163.h"
#include "core/graphics.h"
#include "core/serv_core.h"
#include "core/clock_serv.h"
#include "core/clock_rtc.h"

volatile struct time t;

static void thread1(void *arg); 

int main(void)
{
	serv_init();

	//t.hour = 6;
	//t.minutes = 23;
	//t.second = 0;

	//clock_rtc_set_time(t);

	xTaskCreate(thread1, "thread1", 128, NULL, 1, NULL);
	vTaskStartScheduler();

	while (1)
	{
	}
}

static void thread1(void *arg)
{
	(void)arg;

	ili9163_set_screen(&lcd_conf, BLACK);
	_analog_clock_draw_face();

	//_analog_clock_calculate_and_draw_hand1(t);

	while(1)
	{
		//led_b2_toggle();
		clock_rtc_get_time(&t);
		printf("%d:%d:%d\r\n", t.hour, t.minutes, t.second);
		_analog_clock_update_hand(t);
		vTaskDelay(1000);
	}
}
