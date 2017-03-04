#include "stdio.h"
#include "stdint.h"
#include "system.h"

static void delay(volatile uint32_t count);

int main(void)
{
	system_init();
	
	printf("%s\r\n", __TIME__);

	while (1)
	{
//		printf("Its worked\r\n");
		led_b2_toggle();
		delay(2000000);
	}

}

static void delay(volatile uint32_t count)
{
	while(count--);
}
