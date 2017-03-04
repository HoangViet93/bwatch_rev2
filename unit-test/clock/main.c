#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/spi.h"
#include "core/ili9163.h"
#include "math.h"

static void clock_setup(void);
static void clock_draw_face(void);
static void clock_draw_hand(uint8_t hour, uint8_t min, uint8_t sec);
static uint8_t conv2d(const char* p);

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

uint16_t osx, osy, omx, omy, ohx, ohy;

uint16_t center_x, center_y;
uint16_t radius = 63;
const float scos = 0.0174532925;
uint8_t hh, mm, ss;
uint32_t target_time;

int main(void)
{
	system_init();
	clock_setup();

	ili9163_init(&ili_conf);
	ili9163_set_screen(&ili_conf, 0x0000);

	center_x = ili_conf.lcd_x_size / 2;
	center_y = ili_conf.lcd_y_size / 2;

	hh = conv2d(__TIME__);
	mm = conv2d(__TIME__ + 3);
	ss = conv2d(__TIME__ + 6);

	clock_draw_face();

	target_time = get_tick() + 1000;

	while (1)
	{
		if (target_time < get_tick())
		{
			target_time = get_tick() + 1000;
			ss++;
			if (60 == ss) 
			{
				ss = 0;
				mm ++;
				if (mm > 59)
				{
					mm = 0;
					hh++;
					if (hh > 23)
					{
						hh = 0;
					}
				}
			}
			clock_draw_hand(hh, mm, ss);
		}
	}
}

static void clock_draw_face(void)
{
	uint16_t degree;
	float sx = 0, sy = 1;
	uint16_t x0, x1, yy0, yy1; 

	/* draw outline */
	ili9163_fill_circle(&ili_conf, center_x, center_y, radius, 0xffff);
	ili9163_fill_circle(&ili_conf, center_x, center_y, radius - 4, 0x0000);

	/* draw hour indicator line*/
	for (degree = 0; degree < 360; degree += 30)
	{
		sx = cos((degree - 90) * scos);
		sy = sin((degree - 90) * scos);
		x0 = sx * (radius - 4) + center_x;
		yy0 = sy * (radius - 4) + center_y;
		x1 = sx * (radius - 11) + center_x;
		yy1 = sy * (radius - 11) + center_y;
		ili9163_draw_line(&ili_conf, x0, yy0, x1, yy1, 0xffff);
	}
}

static void clock_draw_hand(uint8_t hour, uint8_t min, uint8_t sec)
{
	float sec_degree, hour_degree, min_degree;
	float sx, sy, hx, hy, mx, my;

	sec_degree = sec * 6;
	min_degree = min * 6 + sec_degree * 0.01666667;
	hour_degree = hour * 30 + min_degree * 0.0833333;

	hx = cos((hour_degree - 90) * scos);
	hy = sin((hour_degree - 90) * scos);
	mx = cos((min_degree - 90) * scos);
	my = sin((min_degree - 90) * scos);
	sx = cos((sec_degree - 90) * scos);
	sy = sin((sec_degree - 90) * scos);

	/* earse old hand */
	ili9163_draw_line(&ili_conf, ohx, ohy, center_x + 1, center_y + 1, 0x0000);
	ili9163_draw_line(&ili_conf, omx, omy, center_x + 1, center_y + 1, 0x0000);
	ili9163_draw_line(&ili_conf, osx, osy, center_x + 1, center_y + 1, 0x0000);
	
	ohx = hx*(radius - 28) + center_x + 1;
	ohy = hy*(radius - 28) + center_y + 1;
	omx = mx*(radius - 17) + center_x + 1;
	omy = my*(radius - 17) + center_y + 1;
	osx = sx*(radius - 14) + center_x + 1;
	osy = sy*(radius - 14) + center_y + 1;

	/* draw new hand */
	ili9163_draw_line(&ili_conf, ohx, ohy, center_x + 1, center_y + 1, 0xffff);
	ili9163_draw_line(&ili_conf, omx, omy, center_x + 1, center_y + 1, 0xffff);
	ili9163_draw_line(&ili_conf, osx, osy, center_x + 1, center_y + 1, 0xf800);

	ili9163_fill_circle(&ili_conf, center_x + 1, center_y + 1, 3, 0xf800);
}

static void clock_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_SPI2);
}

static uint8_t conv2d(const char* p) 
{
	uint8_t v = 0;
	if ('0' <= *p && *p <= '9') 
	{
		v = *p - '0';
	}
	return 10 * v + *++p - '0';
}




