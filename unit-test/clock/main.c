#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/spi.h"
#include "core/ili9163.h"
#include "core/graphics.h"
#include "core/font_data.h"
#include "math.h"
#include "stdlib.h"

static void clock_setup(void);
static void clock_draw_face(void);
static void clock_draw_hand(uint8_t hour, uint8_t min, uint8_t sec);
static uint8_t conv2d(const char* p);

static void clock_draw_hourhand(uint16_t hour_degree);
static void clock_draw_minhand(uint16_t min_degree);

struct font normal_text = 
{
	.width = 7,
	.height = 13,
	.data = DejaVu_Sans_Mono7x13,
	.text_color = WHITE,
	.bkg_color = BLACK
};

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
	//ili9163_draw_line(&ili_conf, (ili_conf.lcd_x_size - 1)/ 2, 0, (ili_conf.lcd_x_size - 1)/ 2, ili_conf.lcd_y_size - 1, 0xf800);
	//ili9163_draw_line(&ili_conf, 0, (ili_conf.lcd_y_size - 1)/ 2, ili_conf.lcd_x_size - 1, (ili_conf.lcd_y_size - 1)/ 2, 0xf800);

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
	uint16_t x0, x1, y0, y1, xd, yd; 
	uint8_t debug = 0;
	const char *dial_num_12 = "12";
	const char *dial_num_3 = "3";
	const char *dial_num_6 = "6";
	const char *dial_num_9 = "9";

	/* draw outline */
	ili9163_fill_circle(&ili_conf, center_x, center_y, radius, BLUE);
	ili9163_fill_circle(&ili_conf, center_x, center_y, radius - 4, 0x0000);

	/* draw hour indicator line*/
	for (degree = 0; degree < 360; degree += 30)
	{
		sx = cos((degree - 90) * scos);
		sy = sin((degree - 90) * scos);
		x0 = sx * (radius - 4) + center_x;
		y0 = sy * (radius - 4) + center_y;
		x1 = sx * (radius - 11) + center_x;
		y1 = sy * (radius - 11) + center_y;
		xd = sx * (radius - 13) + center_x - 8;
		yd = sy * (radius - 13) + center_y - 8;
		if ((degree % 45) == 0)
		{
			//ili9163_print(&ili_conf, xd, yd, dial_num_12, &normal_text);	

			switch (debug)
			{
				case 0:
					ili9163_print(&ili_conf, xd, yd, dial_num_12, &normal_text);
					break;
				case 1:
					ili9163_print(&ili_conf, xd + 8, yd + 2, dial_num_3, &normal_text);
					break;
				case 2:
					ili9163_print(&ili_conf, xd + 5, yd + 5, dial_num_6, &normal_text);
					break;
				case 3:
					ili9163_print(&ili_conf, xd + 3, yd + 3, dial_num_9, &normal_text);
					break;
				default:
					break;
			}

			// if (2 == debug)
			// {
			// 	return;
			// }
			debug++;
		}
		else 
		{
			ili9163_draw_line(&ili_conf, x0, y0, x1, y1, 0xffff);
		}
	}
}

static void clock_draw_hand(uint8_t hour, uint8_t min, uint8_t sec)
{
	float sec_degree, hour_degree, min_degree;
	float sx, sy;

	sec_degree = sec * 6;
	min_degree = min * 6 + sec_degree * 0.01666667;
	hour_degree = hour * 30 + min_degree * 0.0833333;

	clock_draw_hourhand(hour_degree - 90);
	clock_draw_minhand(min_degree - 90);

	//hx = cos((hour_degree - 90) * scos);
	//hy = sin((hour_degree - 90) * scos);
	//mx = cos((min_degree - 90) * scos);
	//my = sin((min_degree - 90) * scos);
	sx = cos((sec_degree - 90) * scos);
	sy = sin((sec_degree - 90) * scos);

	/* earse old hand */
	//ili9163_draw_line(&ili_conf, ohx, ohy, center_x + 1, center_y + 1, 0x0000);
	//ili9163_draw_line(&ili_conf, omx, omy, center_x + 1, center_y + 1, 0x0000);
	ili9163_draw_line(&ili_conf, osx, osy, center_x + 1, center_y + 1, 0x0000);
	
	//ohx = hx*(radius - 28) + center_x + 1;
	//ohy = hy*(radius - 28) + center_y + 1;
	//omx = mx*(radius - 20) + center_x + 1;
	//omy = my*(radius - 20) + center_y + 1;
	osx = sx*(radius - 18) + center_x + 1;
	osy = sy*(radius - 18) + center_y + 1;

	/* draw new hand */
	//ili9163_draw_line(&ili_conf, ohx, ohy, center_x + 1, center_y + 1, 0xffff);
	//ili9163_draw_line(&ili_conf, omx, omy, center_x + 1, center_y + 1, 0xffff);
	ili9163_draw_line(&ili_conf, osx, osy, center_x + 1, center_y + 1, 0xf800);

	ili9163_fill_circle(&ili_conf, center_x + 1, center_y + 1, 3, 0xf800);
}

int16_t old_hourhand_x[4]; 
int16_t old_hourhand_y[4]; 
static void clock_draw_hourhand(uint16_t hour_degree)
{
	float x1, y1, x2, y2, x3, y3, x4, y4;

	x1 = (radius - 28) * cos(hour_degree * scos); 
	y1 = (radius - 28) * sin(hour_degree * scos); 
	x2 = (radius - 60) * cos(hour_degree * scos); 
	y2 = (radius - 60) * sin(hour_degree * scos); 
	x3 = (radius - 45) * cos((hour_degree + 8) * scos); 
	y3 = (radius - 45) * sin((hour_degree + 8) * scos); 
	x4 = (radius - 45) * cos((hour_degree - 8) * scos); 
	y4 = (radius - 45) * sin((hour_degree - 8) * scos); 
	
	ili9163_draw_line(&ili_conf, old_hourhand_x[0], old_hourhand_y[0], 
					  old_hourhand_x[2], old_hourhand_y[2], 0x0000);
	ili9163_draw_line(&ili_conf, old_hourhand_x[2], old_hourhand_y[2], 
					  old_hourhand_x[1], old_hourhand_y[1], 0x0000);
	ili9163_draw_line(&ili_conf, old_hourhand_x[1], old_hourhand_y[1], 
					  old_hourhand_x[3], old_hourhand_y[3], 0x0000);
	ili9163_draw_line(&ili_conf, old_hourhand_x[3], old_hourhand_y[3], 
					  old_hourhand_x[0], old_hourhand_y[0], 0x0000);

	old_hourhand_x[0] = x1 + center_x;
	old_hourhand_x[1] = x2 + center_x;
	old_hourhand_x[2] = x3 + center_x;
	old_hourhand_x[3] = x4 + center_x;

	old_hourhand_y[0] = y1 + center_y;
	old_hourhand_y[1] = y2 + center_y;
	old_hourhand_y[2] = y3 + center_y;
	old_hourhand_y[3] = y4 + center_y;

	ili9163_draw_line(&ili_conf, x1 + center_x, y1 + center_y, 
					  x3 + center_x , y3 + center_y, 0xffff);
	ili9163_draw_line(&ili_conf, x3 + center_x, y3 + center_y, 
					  x2 + center_x , y2 + center_y, 0xffff);
	ili9163_draw_line(&ili_conf, x2 + center_x, y2 + center_y, 
					  x4 + center_x , y4 + center_y, 0xffff);
	ili9163_draw_line(&ili_conf, x4 + center_x, y4 + center_y, 
					  x1 + center_x , y1 + center_y, 0xffff);
}

int16_t old_minhand_x[4]; 
int16_t old_minhand_y[4]; 
static void clock_draw_minhand(uint16_t min_degree)
{
	float x1, y1, x2, y2, x3, y3, x4, y4;

	x1 = (radius - 28 + 8) * cos(min_degree * scos); 
	y1 = (radius - 28 + 8) * sin(min_degree * scos); 
	x2 = (radius - 60) * cos(min_degree * scos); 
	y2 = (radius - 60) * sin(min_degree * scos); 
	x3 = (radius - 45 + 8) * cos((min_degree + 8) * scos); 
	y3 = (radius - 45 + 8) * sin((min_degree + 8) * scos); 
	x4 = (radius - 45 + 8) * cos((min_degree - 8) * scos); 
	y4 = (radius - 45 + 8) * sin((min_degree - 8) * scos); 
	
	ili9163_draw_line(&ili_conf, old_minhand_x[0], old_minhand_y[0], 
					  old_minhand_x[2], old_minhand_y[2], 0x0000);
	ili9163_draw_line(&ili_conf, old_minhand_x[2], old_minhand_y[2], 
					  old_minhand_x[1], old_minhand_y[1], 0x0000);
	ili9163_draw_line(&ili_conf, old_minhand_x[1], old_minhand_y[1], 
					  old_minhand_x[3], old_minhand_y[3], 0x0000);
	ili9163_draw_line(&ili_conf, old_minhand_x[3], old_minhand_y[3], 
					  old_minhand_x[0], old_minhand_y[0], 0x0000);

	old_minhand_x[0] = x1 + center_x;
	old_minhand_x[1] = x2 + center_x;
	old_minhand_x[2] = x3 + center_x;
	old_minhand_x[3] = x4 + center_x;

	old_minhand_y[0] = y1 + center_y;
	old_minhand_y[1] = y2 + center_y;
	old_minhand_y[2] = y3 + center_y;
	old_minhand_y[3] = y4 + center_y;

	ili9163_draw_line(&ili_conf, x1 + center_x, y1 + center_y, 
					  x3 + center_x , y3 + center_y, 0xffff);
	ili9163_draw_line(&ili_conf, x3 + center_x, y3 + center_y, 
					  x2 + center_x , y2 + center_y, 0xffff);
	ili9163_draw_line(&ili_conf, x2 + center_x, y2 + center_y, 
					  x4 + center_x , y4 + center_y, 0xffff);
	ili9163_draw_line(&ili_conf, x4 + center_x, y4 + center_y, 
					  x1 + center_x , y1 + center_y, 0xffff);
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




