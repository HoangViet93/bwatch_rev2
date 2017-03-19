#include "stdint.h"
#include "stdio.h"
#include "system.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"

uint8_t btn_level[2] = {32, 64};
uint8_t btn_oldlevel[2] = {32, 64};
uint8_t count[2];

int main(void)
{
    system_init();

	rcc_periph_clock_enable(RCC_GPIOB);

	gpio_set_mode(GPIOB, GPIO_MODE_INPUT,		
				  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO5);		
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT,		
				  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO6);		

	while (1)
	{
        btn_level[0] = gpio_get(GPIOB, GPIO5); 
        btn_level[1] = gpio_get(GPIOB, GPIO6); 

        if (btn_level[0] != btn_oldlevel[0] && btn_level[0] != 0)
        {
            if (btn_level[0] != 0)
            {
                count[0]++;
                printf("count[0]=%d\r\n", count[0]);
            }
            btn_oldlevel[0] = btn_level[0];
        }

        if (btn_level[1] != btn_oldlevel[1])
        {
            if (btn_level[1] != 0)
            {
                count[1]++;
                printf("count[1]=%d\r\n", count[1]);
            }
            btn_oldlevel[1] = btn_level[1];
        }
        delay_ms(10);
    }
}

