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

#endif

