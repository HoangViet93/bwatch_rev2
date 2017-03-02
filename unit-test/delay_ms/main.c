#include "stdint.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "system.h"

int main(void)
{
	system_init();
	rcc_periph_clock_enable(RCC_GPIOB);

	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,		
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO2);		

	while (1)
	{
		gpio_toggle(GPIOB, GPIO2);
		delay_ms(200);
	}

}

