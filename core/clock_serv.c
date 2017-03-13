#include "core/ili9163.h"
#include "libopencm3/stm32/rcc.h"
#include "core/graphics.h"
#include "core/clock_serv.h"
#include "core/serv_core.h"
#include "core/clock_rtc.h"
#include "math.h"
#include "system.h"

/* debug purpose */
#if (0) || defined(CONFIG_ENABLE_DEBUG) 
#include "stdio.h"
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#define HOUR_UPDATE_IN_MIN  (2) 
#define MIN_UPDATE_IN_SEC   (15)

enum analog_clock_hand
{
	HOUR_HAND = 0,
	MIN_HAND,
	SEC_HAND,
};

static const struct font mono7x13 = 
{
	.width = 7,
	.height = 13,
	.data = DejaVu_Sans_Mono7x13,
	.text_color = WHITE,
	.bkg_color = BLACK
};

static uint16_t center_x;
static uint16_t center_y;
static uint16_t radius;
static const float scos = 0.0174532925; /* pi/2 */

void
clock_serv_init(enum rcc_osc osc)
{
	clock_rtc_init(osc);

	/* pre-calculate size of analog clock */
	center_x = lcd_conf.lcd_x_size / 2;
	center_y = lcd_conf.lcd_y_size / 2;
	radius = center_x - 1;
}

/*----------------------------------------------------------------------------*/
/* analog clock service function */
void
_analog_clock_draw_face(void)
{
	uint16_t degree = 0;
	float sx = 0, sy = 1;
	uint16_t x0, x1, y0, y1, xd, yd; 
	uint8_t num_pos = 0;

	LCD_LOCK();

	/* draw outline */
	ili9163_fill_circle(&lcd_conf, center_x, center_y, radius, BLUE);
	ili9163_fill_circle(&lcd_conf, center_x, center_y, radius - 4, BLACK);

	/* draw hour indicator line and number */
	for (degree = 0; degree < 360; degree += 30)
	{
		/* calculate line indicator position */
		sx = cos((degree - 90) * scos);
		sy = sin((degree - 90) * scos);
		x0 = sx * (radius - 4) + center_x;
		y0 = sy * (radius - 4) + center_y;
		x1 = sx * (radius - 11) + center_x;
		y1 = sy * (radius - 11) + center_y;
		
		/* draw number each 45 degree else draw indicator line */
		if (0 == (degree % 45))
		{
			xd = sx * (radius - 13) + center_x - 8;
			yd = sy * (radius - 13) + center_y - 8;
			
			switch (num_pos)
			{
				case 0:
					ili9163_print(&lcd_conf, xd, yd, "12", &mono7x13);
					break;
				case 1:
					ili9163_print(&lcd_conf, xd + 8, yd + 2, "3", &mono7x13);
					break;
				case 2:
					ili9163_print(&lcd_conf, xd + 5, yd + 5, "6", &mono7x13);
					break;
				case 3:
					ili9163_print(&lcd_conf, xd + 3, yd + 3, "9", &mono7x13);
					break;
				default:
					break;
			}

			num_pos++;
		}
		else 
		{
			ili9163_draw_line(&lcd_conf, x0, y0, x1, y1, 0xffff);
		}
	}

	LCD_UNLOCK();
}

static void
_analog_clock_draw_hand(enum analog_clock_hand type, uint16_t color
						, const int16_t (*hand_x)[4], const int16_t (*hand_y)[4])
{
	if (SEC_HAND == type)
	{
		ili9163_draw_line(&lcd_conf, hand_x[SEC_HAND][0], hand_y[SEC_HAND][0], 
					   	  center_x + 1, center_y + 1, color);
	}
	else
	{
		/* hour and minutes hand draw rhombus style */
		ili9163_draw_line(&lcd_conf, hand_x[type][0], hand_y[type][0], 
						  hand_x[type][2], hand_y[type][2], color);

		ili9163_draw_line(&lcd_conf, hand_x[type][2], hand_y[type][2], 
						  hand_x[type][1], hand_y[type][1], color);
		
		ili9163_draw_line(&lcd_conf, hand_x[type][1], hand_y[type][1], 
						  hand_x[type][3], hand_y[type][3], color);
		
		ili9163_draw_line(&lcd_conf, hand_x[type][3], hand_y[type][3], 
						  hand_x[type][0], hand_y[type][0], color);
	}
}

static void
_analog_clock_calculate_handpos(enum analog_clock_hand type, int16_t degree, 
                                int16_t (*hand_x)[4], int16_t (*hand_y)[4])
{
	uint8_t offset = 0;
	uint8_t i = 0;
	float x[4], y[4];

	if (MIN_HAND == type)
	{
		offset = 8;
	}
	else if (SEC_HAND == type)
	{
		x[0] = cos((degree) * scos);
		y[0] = sin((degree) * scos);

		hand_x[type][0] = x[0]*(radius - 18) + center_x + 1;
		hand_y[type][0] = y[0]*(radius - 18) + center_y + 1;
	}

	/* calculate 4 point to draw rhombus hand style */
	x[0] = (radius - 28 + offset) * cos(degree * scos); 
	y[0] = (radius - 28 + offset) * sin(degree * scos); 
	x[1] = (radius - 60) * cos(degree * scos); 
	y[1] = (radius - 60) * sin(degree * scos); 
	x[2] = (radius - 45 + offset) * cos((degree + 8) * scos); 
	y[2] = (radius - 45 + offset) * sin((degree + 8) * scos); 
	x[3] = (radius - 45 + offset) * cos((degree - 8) * scos); 
	y[3] = (radius - 45 + offset) * sin((degree - 8) * scos); 

	/* get output hand pos value */
	for (i = 0; i < 4; i++)
	{
		hand_x[type][i] = x[i] + center_x;
		hand_y[type][i] = y[i] + center_y;
	}
}

void 
_analog_clock_update_hand(struct time t)
{
	float sec_degree = 0; 
	float hour_degree = 0; 
	float min_degree = 0;
	static int16_t hand_x[3][4];
	static int16_t hand_y[3][4];
	static uint8_t init_draw = 0;

	LCD_LOCK();

	/* calculate degree of each hand */
	sec_degree = t.second * 6;
	/* update min hand each MIN_UPDATE_IN_SEC second else just draw sec hand */
	if (0 == (t.second % MIN_UPDATE_IN_SEC) || 0 == init_draw)
	{
		min_degree = t.minutes * 6 + sec_degree * 0.01666667;
			
		_analog_clock_draw_hand(MIN_HAND, BLACK, hand_x, hand_y);
		_analog_clock_calculate_handpos(MIN_HAND, min_degree - 90, hand_x, hand_y);
		
		/* update hour hand each HOUR_UPDATE_IN_MIN min */
		if (0 == (t.minutes % HOUR_UPDATE_IN_MIN) || 0 == init_draw)
		{
			hour_degree = t.hour * 30 + min_degree * 0.0833333;
			_analog_clock_draw_hand(HOUR_HAND, BLACK, hand_x, hand_y);
			_analog_clock_calculate_handpos(HOUR_HAND, hour_degree - 90, hand_x, hand_y);
		}
		init_draw = 1;
	}
	_analog_clock_draw_hand(SEC_HAND, BLACK, hand_x, hand_y);
	_analog_clock_calculate_handpos(SEC_HAND, sec_degree - 90, hand_x, hand_y);
	
	/* draw new hand */
	_analog_clock_draw_hand(HOUR_HAND, WHITE, hand_x, hand_y);
	_analog_clock_draw_hand(MIN_HAND, WHITE, hand_x, hand_y);
	_analog_clock_draw_hand(SEC_HAND, RED, hand_x, hand_y);

	ili9163_fill_circle(&lcd_conf, center_x + 1, center_y + 1, 3, RED);

	LCD_UNLOCK();
}

/*----------------------------------------------------------------------------*/

