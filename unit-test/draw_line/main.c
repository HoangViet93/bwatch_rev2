#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/spi.h"
#include "core/ili9163.h"

static void clock_setup(void);

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
	delay_ms(2000);
	
	/* cross line */
	ili9163_draw_line(&ili_conf, 0, 0, 127, 127, 0xf800);
	/* vertical line */
	ili9163_draw_line(&ili_conf, 0, 0, 0, 127, 0xf800);
	/* horizontal line */
	ili9163_draw_line(&ili_conf, 0, 0, 127, 0, 0xf800);
	/* fast vline */
	ili9163_draw_fast_vline(&ili_conf, 127, 0, 127, 0xffff);

	while (1)
	{
		led_b2_toggle();	
		delay_ms(1000);
	}
}

static void clock_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_SPI2);
}

