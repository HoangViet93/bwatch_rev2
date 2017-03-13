#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/spi.h"
#include "core/ili9163.h"
#include "core/graphics.h"
#include "core/serv_core.h"

static void clock_setup(void);

const struct font normal_text = 
{
	.width = 7,
	.height = 13,
	.data = DejaVu_Sans_Mono7x13,
	.text_color = WHITE,
	.bkg_color = BLACK
};

int main(void)
{
	serv_init();

	ili9163_set_screen(&lcd_conf, 0x0000);
	
	ili9163_print(&lcd_conf, 0, 0, "1234567890", &normal_text);

	while (1)
	{
		delay_ms(1000);
	}
}

static void clock_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_SPI2);
}

