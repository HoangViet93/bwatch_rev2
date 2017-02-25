#include "stdint.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"

void delay(unsigned long count);

int main(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);

	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,		\ 
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO2);		

	while (1)
	{
		GPIOB_BSRR = GPIO2;
		delay(200000);
		GPIOB_BRR = GPIO2;
		delay(200000);
	}

}

void delay(unsigned long count)
{
    while(count--);
}
