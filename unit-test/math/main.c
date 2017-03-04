#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include "math.h"

#define PI 3.14

extern uint32_t tick_count;

int main(void)
{
	double x, ret, val;
	uint32_t before, after;

	system_init();

	x = 45.0;
	val = PI / 180;

	before = tick_count;
	ret = sin (x * val);
	after = tick_count;

	printf("sin(%lf)=%lf degree\r\n", x, ret);
	printf("before=%d and after=%d\r\n", (int)before, (int)after);
	printf("execute time in ms = %d\r\n", (int)(after - before));

	while (1)
	{
		printf("tick_count=%d\r\n", tick_count);
		delay_ms(1000);
	}

}

