#ifndef ILI9163_H
#define ILI9163_H

#include "stdint.h"
#include "core/graphics.h"
#if defined(CONFIG_FREERTOS)
#include "cmsis_os.h"
#endif

struct ili9163 {
	uint16_t lcd_x_size;
	uint16_t lcd_y_size;

	uint32_t cs_port;
	uint16_t cs_pin;
	uint32_t rst_port;
	uint16_t rst_pin;
	uint32_t a0_port;
	uint16_t a0_pin;
	uint32_t led_port;
	uint16_t led_pin;

	uint32_t spi_reg;
};

#if defined(CONFIG_FREERTOS)
extern SemaphoreHandle_t ili9163_mutex;
#define LCD_LOCK() xSemaphoreTake(ili9163_mutex, 1000)
#define LCD_UNLOCK() xSemaphoreGive(ili9163_mutex)
#else
#define LCD_LOCK()
#define LCD_UNLOCK()
#endif

void ili9163_init(const struct ili9163 *conf);
void ili9163_set_screen(const struct ili9163 *conf, uint16_t color);
int8_t ili9163_draw_pic(const struct ili9163 *conf, uint16_t x, uint16_t y, 
						uint16_t width, uint16_t height, const uint16_t *data);

void ili9163_set_address(const struct ili9163 *conf, 
						 uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);

int8_t ili9163_draw_line(const struct ili9163 *conf, int16_t x0, int16_t y0,
				         int16_t x1, int16_t y1, uint16_t color);
int8_t ili9163_draw_pixel(const struct ili9163 *conf, int16_t x, int16_t y, 
						  uint16_t color);
int8_t ili9163_draw_fast_vline(const struct ili9163 *conf, int16_t x, int16_t y, 
							   int16_t height, uint16_t color);
int8_t ili9163_fill_circle(const struct ili9163 *conf, int16_t x0, int16_t y0, 
			       		   int16_t r, uint16_t color);
int8_t ili9163_put_char(const struct ili9163 *conf, uint16_t x, uint16_t y,
		       			char ch, const struct font *pfont);
uint16_t ili9163_print(const struct ili9163 *conf, uint16_t x, uint16_t y, 
					   const char *str, const struct font *pfont);
#endif

