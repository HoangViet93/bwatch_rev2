#ifndef ILI9163_H
#define ILI9163_H

#include "stdint.h"

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

void ili9163_init(const struct ili9163 *conf);
void ili9163_set_screen(const struct ili9163 *conf, uint16_t color);
int8_t ili9163_draw_pic(const struct ili9163 *conf, uint16_t x, uint16_t y, 
						uint16_t width, uint16_t height, const uint16_t *data);

void ili9163_set_address(const struct ili9163 *conf, 
						 uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);

#endif

