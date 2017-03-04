#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/spi.h"
#include "core/ili9163.h"

static void clock_setup(void);
static void fill_circle(void);

const struct ili9163 ili_conf = 
{
	.lcd_x_size = 128,
	.lcd_y_size = 128,

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
	
	fill_circle();	

	while (1)
	{
		led_b2_toggle();	
		delay_ms(1000);
	}
}

static void fill_circle(void)
{
	uint16_t center_x, center_y;
	uint16_t radius = 63;

	center_x = ili_conf.lcd_x_size / 2;
	center_y = ili_conf.lcd_y_size / 2;

	ili9163_fill_circle(&ili_conf, center_x, center_y, radius, 0xf800);
}

static void clock_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_SPI2);
}

