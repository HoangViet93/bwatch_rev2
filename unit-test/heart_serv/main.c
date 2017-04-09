#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/spi.h"
#include "core/ili9163.h"
#include "core/graphics.h"
#include "core/serv_core.h"
#include "core/heart_serv.h"

static void heart_rate_thread(void *args);

extern struct screen heart_screen; 

int main(void)
{
	serv_init();

	xTaskCreate(heart_rate_thread, "heart_rate_thread", 128, NULL, 1, NULL);
	vTaskStartScheduler();

	while (1)
	{
		delay_ms(1000);
	}
}

static void heart_rate_thread(void *args)
{
	(void)args;

	while(1)
	{
		heart_screen.pfunc();
	}
}
