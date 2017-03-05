#include "core/ili9163.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/spi.h"
#include "system.h"
#include "stdlib.h"
#include "string.h"
#if defined(CONFIG_FREERTOS)
#include "cmsis_os.h"
#endif

static void ili9163_init_communication(const struct ili9163 *conf);
static void ili9163_init_chip(const struct ili9163 *conf);

static void _ili9163_write_byte(const struct ili9163 *conf, uint8_t byte);
static void _ili9163_write_reg(const struct ili9163 *conf, uint8_t reg);
static void _ili9163_write_word(const struct ili9163 *conf, uint16_t word);

#if defined(CONFIG_FREERTOS)
SemaphoreHandle_t ili9163_mutex;
#endif

/*-----------------------------------------------------------------------------*/

void 
ili9163_init(const struct ili9163 *conf)
{
	ili9163_init_communication(conf);
	ili9163_init_chip(conf);

#if defined(CONFIG_FREERTOS)
	ili9163_mutex = xSemaphoreCreateMutex();
#endif

	gpio_set(conf->led_port, conf->led_pin);
}

static void 
ili9163_init_communication(const struct ili9163 *conf)
{	
	/* control pin configuration */
	gpio_set_mode(conf->cs_port, GPIO_MODE_OUTPUT_50_MHZ,		
				  GPIO_CNF_OUTPUT_PUSHPULL, conf->cs_pin);		

	gpio_set_mode(conf->rst_port, GPIO_MODE_OUTPUT_50_MHZ,		
				  GPIO_CNF_OUTPUT_PUSHPULL, conf->rst_pin);		

	gpio_set_mode(conf->a0_port, GPIO_MODE_OUTPUT_50_MHZ,		
				  GPIO_CNF_OUTPUT_PUSHPULL, conf->a0_pin);		

	gpio_set_mode(conf->led_port, GPIO_MODE_OUTPUT_50_MHZ,		
				  GPIO_CNF_OUTPUT_PUSHPULL, conf->led_pin);		

	/* spi configuration */
	if (SPI2 == conf->spi_reg)
	{
		gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,		
					  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO13);		

		gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,		
					  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO15);		
	}

	spi_init_master(conf->spi_reg, SPI_CR1_BAUDRATE_FPCLK_DIV_2, 
					SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE, 
					SPI_CR1_CPHA_CLK_TRANSITION_1,
					SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);	
	spi_enable(conf->spi_reg);
}

static void 
ili9163_init_chip(const struct ili9163 *conf)
{
	gpio_clear(conf->rst_port, conf->rst_pin);
	delay_ms(50);
	gpio_set(conf->rst_port, conf->rst_pin);
	delay_ms(50);

	_ili9163_write_reg(conf, 0x1);
	delay_ms(30);
	_ili9163_write_reg(conf, 0x11);
	delay_ms(120);
	
	/* powerup sequence*/
	_ili9163_write_reg(conf, 0xB1);
	_ili9163_write_byte(conf, 0x05);
	_ili9163_write_byte(conf, 0x3C);
	_ili9163_write_byte(conf, 0x3C);

	_ili9163_write_reg(conf, 0xB2);
	_ili9163_write_byte(conf, 0x05);
	_ili9163_write_byte(conf, 0x3C);
	_ili9163_write_byte(conf, 0x3C);
	
	_ili9163_write_reg(conf, 0xB3);
	_ili9163_write_byte(conf, 0x05);
	_ili9163_write_byte(conf, 0x3C);
	_ili9163_write_byte(conf, 0x3C);
	_ili9163_write_byte(conf, 0x05);
	_ili9163_write_byte(conf, 0x3C);
	_ili9163_write_byte(conf, 0x3C);
	
	_ili9163_write_reg(conf, 0xB4); 
	_ili9163_write_byte(conf, 0x03);
	
	_ili9163_write_reg(conf, 0xC0);
	_ili9163_write_byte(conf, 0x28);
	_ili9163_write_byte(conf, 0x08);
	_ili9163_write_byte(conf, 0x04);
	
	_ili9163_write_reg(conf, 0xC1);
	_ili9163_write_byte(conf, 0XC0);
	
	_ili9163_write_reg(conf, 0xC2);
	_ili9163_write_byte(conf, 0x0D);
	_ili9163_write_byte(conf, 0x00);
	
	_ili9163_write_reg(conf, 0xC3);
	_ili9163_write_byte(conf, 0x8D);
	_ili9163_write_byte(conf, 0x2A);
	
	_ili9163_write_reg(conf, 0xC4);
	_ili9163_write_byte(conf, 0x8D);
	_ili9163_write_byte(conf, 0xEE);	
	
	/* memory access mode */
	_ili9163_write_reg(conf, 0xC5); 
	_ili9163_write_byte(conf, 0x1A);
	
	_ili9163_write_reg(conf, 0x36); 
	_ili9163_write_byte(conf, 0x08);
	
	/* gamma configuration */
	_ili9163_write_reg(conf, 0xE0);
	_ili9163_write_byte(conf, 0x04);
	_ili9163_write_byte(conf, 0x22);
	_ili9163_write_byte(conf, 0x07);
	_ili9163_write_byte(conf, 0x0A);
	_ili9163_write_byte(conf, 0x2E);
	_ili9163_write_byte(conf, 0x30);
	_ili9163_write_byte(conf, 0x25);
	_ili9163_write_byte(conf, 0x2A);
	_ili9163_write_byte(conf, 0x28);
	_ili9163_write_byte(conf, 0x26);
	_ili9163_write_byte(conf, 0x2E);
	_ili9163_write_byte(conf, 0x3A);
	_ili9163_write_byte(conf, 0x00);
	_ili9163_write_byte(conf, 0x01);
	_ili9163_write_byte(conf, 0x03);
	_ili9163_write_byte(conf, 0x13);
	
	_ili9163_write_reg(conf, 0xE1);
	_ili9163_write_byte(conf, 0x04);
	_ili9163_write_byte(conf, 0x16);
	_ili9163_write_byte(conf, 0x06);
	_ili9163_write_byte(conf, 0x0D);
	_ili9163_write_byte(conf, 0x2D);
	_ili9163_write_byte(conf, 0x26);
	_ili9163_write_byte(conf, 0x23);
	_ili9163_write_byte(conf, 0x27);
	_ili9163_write_byte(conf, 0x27);
	_ili9163_write_byte(conf, 0x25);
	_ili9163_write_byte(conf, 0x2D);
	_ili9163_write_byte(conf, 0x3B);
	_ili9163_write_byte(conf, 0x00);
	_ili9163_write_byte(conf, 0x01);
	_ili9163_write_byte(conf, 0x04);
	_ili9163_write_byte(conf, 0x13);
	
	/* dipslay config */
	_ili9163_write_reg(conf, 0x3A);
	_ili9163_write_byte(conf, 0x05);
	_ili9163_write_reg(conf, 0x29); 
}

/*-----------------------------------------------------------------------------*/

void
ili9163_set_address(const struct ili9163 *conf, 
					uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2)
{
	_ili9163_write_reg(conf, 0x2A);
	_ili9163_write_word(conf, x1);
	_ili9163_write_word(conf, x2);

	_ili9163_write_reg(conf, 0x2B);
	_ili9163_write_word(conf, y1);
	_ili9163_write_word(conf, y2);

	_ili9163_write_reg(conf, 0x2C);
}

void
ili9163_set_screen(const struct ili9163 *conf, uint16_t color)
{
	uint16_t row_index, col_index;

	ili9163_set_address(conf, 0, conf->lcd_x_size - 1, 0, conf->lcd_y_size - 1);

	for (row_index = 0; row_index < conf->lcd_y_size; row_index++)
	{
		for (col_index = 0; col_index < conf->lcd_x_size; col_index++)
		{
			_ili9163_write_word(conf, color);
		}
	}
}

int8_t
ili9163_draw_pic(const struct ili9163 *conf, uint16_t x, uint16_t y, 
				 uint16_t width, uint16_t height, const uint16_t *data)
{
	uint32_t pic_index; 

	if ((x + width) >= conf->lcd_x_size || (y + height) >= conf->lcd_y_size)
	{
		return -1;
	}

	ili9163_set_address(conf, x, x + width - 1, y, y + height - 1);
	for (pic_index = 0; pic_index < width * height; pic_index++)
	{
		_ili9163_write_word(conf, data[pic_index]);
	}

	return 0;
}

int8_t
ili9163_draw_pixel(const struct ili9163 *conf, int16_t x, int16_t y, 
				   uint16_t color)
{
	if (x >= conf->lcd_x_size || y >= conf->lcd_y_size 
		|| x < 0 || y < 0)
	{
		return -1;
	}

	ili9163_set_address(conf, x, x + 1, y, y + 1);
	_ili9163_write_word(conf, color);

	return 0;
}

static inline void 
_swap_int16_t(int16_t *a, int16_t *b)
{
	int16_t temp = *a;
	*a = *b;
	*b = temp;
}

/* bresenham's algorithm */
int8_t
ili9163_draw_line(const struct ili9163 *conf, int16_t x0, int16_t y0,
				  int16_t x1, int16_t y1, uint16_t color)
{
	int16_t steep = 0;
	int16_t dx, dy;
	int16_t error, y_step;

	if (x0 >= conf->lcd_x_size || y0 >= conf->lcd_y_size
		|| x1 >= conf->lcd_x_size || y1 >= conf->lcd_y_size)
	{
		return -1;
	}

	steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep)
	{
		_swap_int16_t(&x0, &y0);
		_swap_int16_t(&x1, &y1);
	}
	if (x0 > x1)
	{
		_swap_int16_t(&x0, &x1);
		_swap_int16_t(&y0, &y1);
	}

	dx = x1 - x0;
	dy = abs(y1 - y0);

	error = dx / 2;
	if (y0 < y1)
	{
		y_step = 1;
	}
	else {
		y_step = -1;
	}

	for (; x0 <= x1; x0++)
	{
		if (steep) 
		{
			ili9163_draw_pixel(conf, y0, x0, color);
		}
		else 
		{
			ili9163_draw_pixel(conf, x0, y0, color);
		}
		error -= dy;
		if (error < 0) 
		{
			y0 += y_step;
			error += dx;
		}
	}

	return 0;
}

int8_t 
ili9163_draw_fast_vline(const struct ili9163 *conf, int16_t x, int16_t y, 
					    int16_t height, uint16_t color)
{
	if (x >= conf->lcd_x_size || y >= conf->lcd_y_size)
	{
		return -1;
	}
	if ((y + height) - 1 > conf->lcd_y_size)
	{
		height = conf->lcd_y_size - y;
	}

	ili9163_set_address(conf, x, x, y, (y + height) - 1);
	
	while(height)
	{
		_ili9163_write_word(conf, color);
		height--;
	}
	return 0;
}

static void
_ili9163_fill_circle_helper(const struct ili9163 *conf, int16_t x0, int16_t y0, 
							int16_t r, uint8_t corner_name, 
							int16_t delta, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddf_x = 1;
	int16_t ddf_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y)
	{
		if (f >= 0) 
		{
			y--;
			ddf_y += 2;
			f += ddf_y;
		}
		x++;
		ddf_x += 2;
		f += ddf_x;

		if (corner_name & 0x01) 
		{
			ili9163_draw_fast_vline(conf, x0 + x, y0 - y, 2 * y + 1 + delta, color);
			ili9163_draw_fast_vline(conf, x0 + y, y0 - x, 2 * x + 1 + delta, color);
		}
		if (corner_name & 0x02)
		{
			ili9163_draw_fast_vline(conf, x0 - x, y0 - y, 2 * y + 1 + delta, color);
			ili9163_draw_fast_vline(conf, x0 - y, y0 - x, 2 * x + 1 + delta, color);
		}
	}
}

int8_t
ili9163_fill_circle(const struct ili9163 *conf, int16_t x0, int16_t y0, 
					int16_t r, uint16_t color)
{
	if (x0 + r >= conf->lcd_x_size || y0 + r >= conf->lcd_y_size)
	{
		return -1;
	}
	ili9163_draw_fast_vline(conf, x0, y0 - r, 2*r + 1, color);
	_ili9163_fill_circle_helper(conf, x0, y0, r, 3, 0, color);

	return 0;
}

/* I can't remember how i code it */
int8_t
ili9163_put_char(const struct ili9163 *conf, uint16_t x, uint16_t y,
				 char ch, const struct font *pfont)
{
	uint16_t char_width, temp = 0, byte_height, start_char;
	uint16_t x_count, y_count;
	
	if (x + pfont->width >= conf->lcd_x_size 
		|| y + pfont->height >= conf->lcd_y_size)
	{
		return -1;
	}

	if ((pfont->height % 8) > 0)
	{
		byte_height = pfont->height / 8 + 1;
	}
	else 
	{
		byte_height = pfont->height / 8;
	}
	
	start_char = (ch - 32) * (pfont->width * byte_height + 1) + 1;
	char_width = pfont->data[start_char - 1];

	ili9163_set_address(conf, x, x + char_width - 1, y, y + pfont->height - 1);

	for (y_count = 0; y_count < pfont->height; )
	{
		for (x_count = 0; x_count < char_width; x_count++)
		{
			if ((pfont->data[start_char + (byte_height * x_count)]
				 >> (y_count - 8 * temp)) & 0x01 )
			{
				_ili9163_write_word(conf, pfont->text_color);
			}
			else 
			{
				_ili9163_write_word(conf, pfont->bkg_color);
			}
		}
		y_count++;
		temp = y_count / 8;
		if (y_count == 8 * temp)
		{
			start_char++;
		}
	}

	return 0;
}

uint16_t 
ili9163_print(const struct ili9163 *conf, uint16_t x, uint16_t y, 
			  const char *str, const struct font *pfont)
{ 
	uint8_t len;
	uint8_t byte_height;
	uint16_t x1 = 0;
	volatile uint16_t count, temp;

	len = strlen(str);

	if ((pfont->height % 8) > 0)
	{
		byte_height = pfont->height / 8 + 1;
	}
	else 
	{
		byte_height = pfont->height / 8;
	}

	for (count = 0; count < len; count++)
	{
		if (-1 == ili9163_put_char(conf, x + x1, y, str[count], pfont))
		{
			return count;
		}
		temp = (str[count] - 32) * (pfont->width * byte_height + 1);
		x1 += pfont->data[temp];
		temp = (str[count + 1] - 32) * (pfont->width * byte_height + 1);

		if ((x + x1 + pfont->data[temp]) >= (conf->lcd_x_size - 1))
		{
			y += pfont->height;
			x1 = 0;
		}
	}
	return len;
}

/*-----------------------------------------------------------------------------*/

static void
_ili9163_write_byte(const struct ili9163 *conf, uint8_t byte)
{
	gpio_set(conf->a0_port, conf->a0_pin);
	gpio_clear(conf->cs_port, conf->cs_pin);
	
	while (!(SPI_SR(conf->spi_reg) & SPI_SR_TXE));
	spi_xfer(conf->spi_reg, byte);

	gpio_set(conf->cs_port, conf->cs_pin);
}

static void
_ili9163_write_reg(const struct ili9163 *conf, uint8_t reg)
{
	gpio_clear(conf->a0_port, conf->a0_pin);
	gpio_clear(conf->cs_port, conf->cs_pin);
	
	while (!(SPI_SR(conf->spi_reg) & SPI_SR_TXE));
	spi_xfer(conf->spi_reg, reg);

	gpio_set(conf->cs_port, conf->cs_pin);
}

static void
_ili9163_write_word(const struct ili9163 *conf, uint16_t word)
{
	gpio_set(conf->a0_port, conf->a0_pin);
	gpio_clear(conf->cs_port, conf->cs_pin);

	_ili9163_write_byte(conf, word >> 8);	
	_ili9163_write_byte(conf, word);

	gpio_set(conf->cs_port, conf->cs_pin);
}

