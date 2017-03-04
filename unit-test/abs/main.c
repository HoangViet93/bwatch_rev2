#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include "stdlib.h"

int main(void)
{
	int a, b;

	system_init();

	a = abs(5);
	printf("a=%d\r\n", a);

	b = abs(-10);
	printf("b=%d\r\n", b);

	while (1)
	{
	}

}

