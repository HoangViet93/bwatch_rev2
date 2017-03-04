#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/spi.h"
#include "core/ili9163.h"
#include "core/graphics.h"
#include "core/font_data.h"

static void clock_setup(void);

const struct font normal_text = 
{
	.width = 15,
	.height = 16,
	.data = arial_15x16,
	.text_color = WHITE,
	.bkg_color = BLACK
};


const struct ili9163 ili_conf = 
{
	.lcd_x_size = 128,
	.lcd_y_size = 160,

	.cs_port =	GPIOB,
	.cs_pin =	GPIO12,
	.rst_port = GPIOA,
	.rst_pin =	GPIO3,
	.a0_port =	GPIOA,
	.a0_pin =	GPIO2,
	.led_port = GPIOB,
	.led_pin =	GPIO14,

	.spi_reg = SPI2 
};

int main(void)
{
	system_init();
	clock_setup();

	ili9163_init(&ili_conf);
	ili9163_set_screen(&ili_conf, 0x0000);
	
	ili9163_print(&ili_conf, 0, 0, "hello", &normal_text);

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

