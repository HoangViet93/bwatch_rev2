#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "stdint.h"

enum color_rgb565
{
	BLACK   = 0x0000,
	BLUE    = 0x001F,
	RED     = 0xF800,
	GREEN   = 0x07E0,
	CYAN    = 0x07FF,
	MAGENTA = 0xF81F,
	YELLOW  = 0xFFE0,  
	WHITE   = 0xFFFF
};

struct font 
{
	uint8_t width;
	uint8_t height;
	const uint8_t* data;
	uint16_t text_color;
	uint16_t bkg_color;
};

extern const uint8_t DejaVu_Sans_Mono7x13[];
extern const uint8_t Digital_Mono12x24[];
extern const uint8_t Calibri18x20[];
extern const uint16_t heartcolor32x32[1024];

uint16_t font_get_str_width(const struct font *pfont, const char *str);
uint16_t font_get_str_height(uint16_t lcd_x_size, const struct font *pfont, const char *str);

#endif

