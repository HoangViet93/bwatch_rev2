#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/spi.h"
#include "core/ili9163.h"
#include "core/graphics.h"
#include "core/serv_core.h"

int main(void)
{
	serv_init();

    digi_clock_init();

	while (1)
	{
        digi_clock_update();
		delay_ms(1000);
	}
}


