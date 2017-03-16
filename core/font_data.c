#include "stdint.h"
#include "string.h"
#include "core/graphics.h"

uint16_t font_get_str_width(const struct font *pfont, const char *str)
{
    uint16_t byte_height = 0;
    uint16_t start_char = 0;
    uint16_t str_width = 0;
    uint16_t i = 0;

    if ((pfont->height % 8) > 0)
	{
		byte_height = pfont->height / 8 + 1;
	}
	else 
	{
		byte_height = pfont->height / 8;
	}

    for (i = 0; i < strlen(str); i++)
    {
	    start_char = (str[i] - 32) * (pfont->width * byte_height + 1) + 1;
	    str_width += pfont->data[start_char - 1];
    }

    return str_width;
}

uint16_t font_get_str_height(uint16_t lcd_x_size, const struct font *pfont, const char *str)
{
    uint16_t str_width = 0;
    uint16_t str_height = 0;
    uint8_t x_ovf = 0;

    str_width = font_get_str_width(pfont, str);
    x_ovf = str_width / lcd_x_size;
    if (x_ovf > 0)
    {
        str_height = x_ovf * pfont->height;
    }
    else 
    {
        str_height = pfont->height;
    }

    return str_height;
}

//WARNING: This Font Require X-GLCD Lib.
//         You can not use it with MikroE GLCD Lib.

//Font Generated by MikroElektronika GLCD Font Creator 1.2.0.0
//MikroElektronika 2011 
//http://www.mikroe.com 

//GLCD FontName : DejaVu_Sans_Mono7x13
//GLCD FontSize : 7 x 13

const uint8_t DejaVu_Sans_Mono7x13[] = 
{
        0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char  
        0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x03, 0x7E, 0x03, 0x00, 0x00, 0x00, 0x00,  // Code for char !
        0x07, 0x00, 0x00, 0x0E, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x0E, 0x00,  // Code for char "
        0x07, 0x80, 0x00, 0x90, 0x03, 0xF8, 0x00, 0x9C, 0x03, 0xF0, 0x01, 0x9C, 0x00, 0x10, 0x00,  // Code for char #
        0x07, 0x18, 0x01, 0x3C, 0x03, 0x24, 0x02, 0xFE, 0x0F, 0x24, 0x02, 0xEC, 0x03, 0xC0, 0x01,  // Code for char $
        0x07, 0x4C, 0x00, 0x52, 0x00, 0x52, 0x00, 0xAC, 0x01, 0x60, 0x02, 0x50, 0x02, 0x90, 0x01,  // Code for char %
        0x07, 0x00, 0x00, 0xC0, 0x01, 0xEE, 0x03, 0x3E, 0x02, 0xE2, 0x03, 0x80, 0x03, 0xE0, 0x02,  // Code for char &
        0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char '
        0x05, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0xFE, 0x03, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00,  // Code for char (
        0x05, 0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 0xFE, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char )
        0x06, 0x00, 0x00, 0x24, 0x00, 0x18, 0x00, 0x7E, 0x00, 0x18, 0x00, 0x24, 0x00, 0x00, 0x00,  // Code for char *
        0x07, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0xF8, 0x03, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00,  // Code for char +
        0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,  // Code for char ,
        0x06, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60, 0x00, 0x60, 0x00, 0x00, 0x00,  // Code for char -
        0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,  // Code for char .
        0x07, 0x00, 0x00, 0x00, 0x04, 0x00, 0x03, 0xC0, 0x00, 0x30, 0x00, 0x0C, 0x00, 0x02, 0x00,  // Code for char /
        0x07, 0x00, 0x00, 0xF8, 0x00, 0xFE, 0x03, 0x02, 0x02, 0x22, 0x02, 0xFE, 0x03, 0xF8, 0x00,  // Code for char 0
        0x07, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0xFE, 0x03, 0xFE, 0x03, 0x00, 0x02, 0x00, 0x02,  // Code for char 1
        0x07, 0x00, 0x00, 0x04, 0x03, 0x82, 0x03, 0xC2, 0x02, 0x62, 0x02, 0x3E, 0x02, 0x1C, 0x02,  // Code for char 2
        0x07, 0x00, 0x00, 0x04, 0x01, 0x02, 0x02, 0x22, 0x02, 0x22, 0x02, 0xDE, 0x03, 0xDC, 0x01,  // Code for char 3
        0x07, 0x00, 0x00, 0xE0, 0x00, 0xB0, 0x00, 0x8C, 0x00, 0xFE, 0x03, 0xFE, 0x03, 0x80, 0x00,  // Code for char 4
        0x07, 0x00, 0x00, 0x1E, 0x01, 0x1E, 0x02, 0x12, 0x02, 0x32, 0x02, 0xF2, 0x03, 0xE0, 0x01,  // Code for char 5
        0x07, 0x00, 0x00, 0xF8, 0x01, 0xFE, 0x03, 0x16, 0x02, 0x12, 0x02, 0xF2, 0x03, 0xE0, 0x01,  // Code for char 6
        0x07, 0x00, 0x00, 0x02, 0x00, 0x02, 0x02, 0xC2, 0x03, 0xFA, 0x01, 0x3E, 0x00, 0x0E, 0x00,  // Code for char 7
        0x07, 0x00, 0x00, 0xDC, 0x01, 0xDE, 0x03, 0x22, 0x02, 0x22, 0x02, 0xDE, 0x03, 0xDC, 0x01,  // Code for char 8
        0x07, 0x00, 0x00, 0x3C, 0x00, 0x7E, 0x02, 0x42, 0x02, 0x42, 0x03, 0xFE, 0x03, 0xFC, 0x00,  // Code for char 9
        0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x03, 0x30, 0x03, 0x00, 0x00, 0x00, 0x00,  // Code for char :
        0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x30, 0x07, 0x30, 0x03, 0x00, 0x00, 0x00, 0x00,  // Code for char ;
        0x07, 0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0xF0, 0x00, 0x90, 0x00, 0x90, 0x00, 0x98, 0x01,  // Code for char <
        0x07, 0x00, 0x00, 0xA0, 0x00, 0xA0, 0x00, 0xA0, 0x00, 0xA0, 0x00, 0xA0, 0x00, 0xA0, 0x00,  // Code for char =
        0x07, 0x00, 0x00, 0x98, 0x01, 0x90, 0x00, 0x90, 0x00, 0xF0, 0x00, 0x60, 0x00, 0x60, 0x00,  // Code for char >
        0x07, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x62, 0x03, 0x72, 0x03, 0x1E, 0x00, 0x0C, 0x00,  // Code for char ?
        0x07, 0xF0, 0x01, 0x18, 0x06, 0xE4, 0x0D, 0x14, 0x0A, 0x14, 0x0A, 0x14, 0x0A, 0xF8, 0x0F,  // Code for char @
        0x07, 0x00, 0x00, 0x00, 0x03, 0xF0, 0x03, 0xBE, 0x00, 0xBE, 0x00, 0xF0, 0x03, 0x00, 0x03,  // Code for char A
        0x07, 0x00, 0x00, 0xFE, 0x03, 0xFE, 0x03, 0x22, 0x02, 0x22, 0x02, 0xDE, 0x03, 0xDC, 0x01,  // Code for char B
        0x07, 0x00, 0x00, 0xF8, 0x00, 0xFC, 0x01, 0x06, 0x03, 0x02, 0x02, 0x02, 0x02, 0x04, 0x01,  // Code for char C
        0x07, 0x00, 0x00, 0xFE, 0x03, 0xFE, 0x03, 0x02, 0x02, 0x06, 0x03, 0xFC, 0x01, 0xF8, 0x00,  // Code for char D
        0x07, 0x00, 0x00, 0xFE, 0x03, 0xFE, 0x03, 0x22, 0x02, 0x22, 0x02, 0x22, 0x02, 0x02, 0x02,  // Code for char E
        0x07, 0x00, 0x00, 0xFE, 0x03, 0xFE, 0x03, 0x22, 0x00, 0x22, 0x00, 0x22, 0x00, 0x02, 0x00,  // Code for char F
        0x07, 0x00, 0x00, 0xF8, 0x00, 0xFC, 0x01, 0x06, 0x03, 0x42, 0x02, 0xC2, 0x03, 0xC4, 0x03,  // Code for char G
        0x07, 0x00, 0x00, 0xFE, 0x03, 0xFE, 0x03, 0x20, 0x00, 0x20, 0x00, 0xFE, 0x03, 0xFE, 0x03,  // Code for char H
        0x07, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0xFE, 0x03, 0xFE, 0x03, 0x02, 0x02, 0x02, 0x02,  // Code for char I
        0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0xFE, 0x03, 0xFE, 0x01,  // Code for char J
        0x07, 0x00, 0x00, 0xFE, 0x03, 0xFE, 0x03, 0x30, 0x00, 0x78, 0x00, 0xCE, 0x01, 0x06, 0x03,  // Code for char K
        0x07, 0x00, 0x00, 0xFE, 0x03, 0xFE, 0x03, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02,  // Code for char L
        0x07, 0x00, 0x00, 0xFE, 0x03, 0xFC, 0x03, 0x38, 0x00, 0x38, 0x00, 0xFC, 0x03, 0xFE, 0x03,  // Code for char M
        0x07, 0x00, 0x00, 0xFE, 0x03, 0xFE, 0x03, 0x3C, 0x00, 0xE0, 0x01, 0xFE, 0x03, 0xFE, 0x03,  // Code for char N
        0x07, 0x00, 0x00, 0xF8, 0x00, 0xFE, 0x03, 0x02, 0x02, 0x02, 0x02, 0xFE, 0x03, 0xF8, 0x00,  // Code for char O
        0x07, 0x00, 0x00, 0xFE, 0x03, 0xFE, 0x03, 0x22, 0x00, 0x22, 0x00, 0x3E, 0x00, 0x1C, 0x00,  // Code for char P
        0x07, 0x00, 0x00, 0xF8, 0x00, 0xFE, 0x03, 0x02, 0x02, 0x02, 0x02, 0xFE, 0x07, 0xF8, 0x00,  // Code for char Q
        0x07, 0x00, 0x00, 0xFE, 0x03, 0xFE, 0x03, 0x22, 0x00, 0x62, 0x00, 0xDE, 0x01, 0x9C, 0x03,  // Code for char R
        0x07, 0x00, 0x00, 0x1C, 0x01, 0x3E, 0x02, 0x32, 0x02, 0x62, 0x02, 0xE2, 0x03, 0xC4, 0x01,  // Code for char S
        0x07, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0xFE, 0x03, 0xFE, 0x03, 0x02, 0x00, 0x02, 0x00,  // Code for char T
        0x07, 0x00, 0x00, 0xFE, 0x01, 0xFE, 0x03, 0x00, 0x02, 0x00, 0x02, 0xFE, 0x03, 0xFE, 0x01,  // Code for char U
        0x07, 0x00, 0x00, 0x06, 0x00, 0xFE, 0x00, 0xE0, 0x03, 0xE0, 0x03, 0xFE, 0x00, 0x06, 0x00,  // Code for char V
        0x07, 0x1E, 0x00, 0xFE, 0x03, 0xE0, 0x03, 0x18, 0x00, 0xE0, 0x03, 0xFE, 0x03, 0x3E, 0x00,  // Code for char W
        0x07, 0x00, 0x00, 0x02, 0x02, 0x8E, 0x03, 0xF8, 0x00, 0xF8, 0x00, 0x8E, 0x03, 0x02, 0x02,  // Code for char X
        0x07, 0x02, 0x00, 0x0E, 0x00, 0x3C, 0x00, 0xF0, 0x03, 0xF0, 0x03, 0x3C, 0x00, 0x0E, 0x00,  // Code for char Y
        0x07, 0x00, 0x00, 0x02, 0x03, 0xC2, 0x03, 0xE2, 0x02, 0x3A, 0x02, 0x1E, 0x02, 0x06, 0x02,  // Code for char Z
        0x05, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x07, 0xFF, 0x07, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00,  // Code for char [
        0x07, 0x00, 0x00, 0x02, 0x00, 0x0E, 0x00, 0x30, 0x00, 0xC0, 0x00, 0x00, 0x07, 0x00, 0x04,  // Code for char BackSlash
        0x05, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0xFF, 0x07, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00,  // Code for char ]
        0x07, 0x08, 0x00, 0x0C, 0x00, 0x06, 0x00, 0x02, 0x00, 0x06, 0x00, 0x0C, 0x00, 0x08, 0x00,  // Code for char ^
        0x07, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10,  // Code for char _
        0x04, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char `
        0x07, 0x00, 0x00, 0x80, 0x01, 0xD0, 0x03, 0x48, 0x02, 0x48, 0x02, 0xF8, 0x03, 0xF0, 0x03,  // Code for char a
        0x07, 0x00, 0x00, 0xFF, 0x03, 0xFF, 0x03, 0x08, 0x02, 0x08, 0x02, 0xF8, 0x03, 0xF0, 0x01,  // Code for char b
        0x07, 0x00, 0x00, 0xE0, 0x00, 0xF0, 0x01, 0x18, 0x03, 0x08, 0x02, 0x08, 0x02, 0x10, 0x01,  // Code for char c
        0x07, 0x00, 0x00, 0xF0, 0x01, 0xF8, 0x03, 0x08, 0x02, 0x08, 0x02, 0xFF, 0x03, 0xFF, 0x03,  // Code for char d
        0x07, 0x00, 0x00, 0xF0, 0x01, 0xF8, 0x03, 0x48, 0x02, 0x48, 0x02, 0x78, 0x02, 0x70, 0x01,  // Code for char e
        0x07, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0xFE, 0x03, 0xFF, 0x03, 0x09, 0x00, 0x09, 0x00,  // Code for char f
        0x07, 0x00, 0x00, 0xF0, 0x01, 0xF8, 0x13, 0x08, 0x12, 0x08, 0x12, 0xF8, 0x1F, 0xF8, 0x0F,  // Code for char g
        0x07, 0x00, 0x00, 0xFF, 0x03, 0xFF, 0x03, 0x08, 0x00, 0x08, 0x00, 0xF8, 0x03, 0xF0, 0x03,  // Code for char h
        0x07, 0x00, 0x00, 0x08, 0x02, 0x08, 0x02, 0xFB, 0x03, 0xFB, 0x03, 0x00, 0x02, 0x00, 0x02,  // Code for char i
        0x05, 0x00, 0x00, 0x08, 0x10, 0x08, 0x10, 0xFB, 0x1F, 0xFB, 0x0F, 0x00, 0x00, 0x00, 0x00,  // Code for char j
        0x07, 0x00, 0x00, 0xFF, 0x03, 0xFF, 0x03, 0x60, 0x00, 0xF0, 0x01, 0x98, 0x03, 0x00, 0x02,  // Code for char k
        0x06, 0x01, 0x00, 0x01, 0x00, 0xFF, 0x01, 0xFF, 0x03, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00,  // Code for char l
        0x07, 0x00, 0x00, 0xF8, 0x03, 0xF8, 0x03, 0x08, 0x00, 0xF8, 0x03, 0x08, 0x00, 0xF8, 0x03,  // Code for char m
        0x07, 0x00, 0x00, 0xF8, 0x03, 0xF8, 0x03, 0x08, 0x00, 0x08, 0x00, 0xF8, 0x03, 0xF0, 0x03,  // Code for char n
        0x07, 0x00, 0x00, 0xF0, 0x01, 0xF8, 0x03, 0x08, 0x02, 0x08, 0x02, 0xF8, 0x03, 0xF0, 0x01,  // Code for char o
        0x07, 0x00, 0x00, 0xF8, 0x1F, 0xF8, 0x1F, 0x08, 0x02, 0x08, 0x02, 0xF8, 0x03, 0xF0, 0x01,  // Code for char p
        0x07, 0x00, 0x00, 0xF0, 0x01, 0xF8, 0x03, 0x08, 0x02, 0x08, 0x02, 0xF8, 0x1F, 0xF8, 0x1F,  // Code for char q
        0x07, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x03, 0xF8, 0x03, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00,  // Code for char r
        0x07, 0x00, 0x00, 0x30, 0x01, 0x78, 0x02, 0x68, 0x02, 0x48, 0x02, 0xC8, 0x03, 0x90, 0x01,  // Code for char s
        0x06, 0x08, 0x00, 0x08, 0x00, 0xFE, 0x01, 0xFE, 0x03, 0x08, 0x02, 0x08, 0x02, 0x00, 0x00,  // Code for char t
        0x07, 0x00, 0x00, 0xF8, 0x01, 0xF8, 0x03, 0x00, 0x02, 0x00, 0x02, 0xF8, 0x03, 0xF8, 0x03,  // Code for char u
        0x07, 0x00, 0x00, 0x18, 0x00, 0xF8, 0x00, 0xC0, 0x03, 0xC0, 0x03, 0xF8, 0x00, 0x18, 0x00,  // Code for char v
        0x07, 0x38, 0x00, 0xF8, 0x03, 0xC0, 0x03, 0x20, 0x00, 0xC0, 0x03, 0xF8, 0x03, 0x38, 0x00,  // Code for char w
        0x07, 0x00, 0x00, 0x08, 0x02, 0xB8, 0x03, 0xF0, 0x01, 0xF0, 0x01, 0xB8, 0x03, 0x08, 0x02,  // Code for char x
        0x07, 0x00, 0x00, 0x18, 0x10, 0xF8, 0x10, 0xC0, 0x1F, 0xC0, 0x07, 0xF8, 0x00, 0x18, 0x00,  // Code for char y
        0x07, 0x00, 0x00, 0x08, 0x03, 0x88, 0x03, 0xC8, 0x02, 0x68, 0x02, 0x38, 0x02, 0x18, 0x02,  // Code for char z
        0x07, 0x00, 0x00, 0x20, 0x00, 0x20, 0x00, 0xDF, 0x03, 0xDF, 0x07, 0x01, 0x04, 0x01, 0x04,  // Code for char {
        0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char |
        0x07, 0x00, 0x00, 0x01, 0x04, 0x01, 0x04, 0xDF, 0x07, 0xDF, 0x07, 0x20, 0x00, 0x20, 0x00,  // Code for char }
        0x07, 0x00, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00,  // Code for char ~
        0x04, 0xFC, 0x03, 0xFC, 0x03, 0xFC, 0x03, 0xFC, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // Code for char 
};




//WARNING: This Font Require X-GLCD Lib.
//         You can not use it with MikroE GLCD Lib.

//Font Generated by MikroElektronika GLCD Font Creator 1.2.0.0
//MikroElektronika 2011 
//http://www.mikroe.com 

//GLCD FontName : Digital-7_Mono12x24
//GLCD FontSize : 12 x 24

const uint8_t Digital_Mono12x24[] = {
        0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char  
        0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xE7, 0x19, 0xF8, 0xFF, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char !
        0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char "
        0x0B, 0x00, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x66, 0x00, 0xC0, 0xB9, 0x03, 0xC0, 0x99, 0x03, 0x00, 0x66, 0x00, 0x00, 0x66, 0x00, 0xC0, 0xB9, 0x03, 0xC0, 0x99, 0x03, 0x00, 0x66, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00,  // Code for char #
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0x0F, 0x10, 0xF8, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xC0, 0x0F, 0x18, 0xE0, 0x1F, 0x00, 0x00, 0x00,  // Code for char $
        0x0C, 0xE8, 0x01, 0x00, 0xF8, 0x00, 0x18, 0x98, 0x01, 0x1E, 0x98, 0x81, 0x0F, 0xF0, 0xE1, 0x03, 0x78, 0xF1, 0x00, 0x00, 0x8F, 0x1E, 0xC0, 0x87, 0x0F, 0xF0, 0x81, 0x19, 0x78, 0x80, 0x19, 0x18, 0x00, 0x1F, 0x00, 0x80, 0x17,  // Code for char %
        0x0C, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x17, 0xF0, 0xFF, 0x1F, 0xF0, 0x1B, 0x18, 0x30, 0x18, 0x18, 0x30, 0x18, 0x18, 0x30, 0x18, 0x18, 0x30, 0x18, 0x18, 0xE0, 0x1B, 0x18, 0xF0, 0xF7, 0x3B, 0x00, 0xF0, 0x7F, 0x00, 0x00, 0x10,  // Code for char &
        0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char '
        0x07, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x07, 0xE0, 0xE7, 0x0F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char (
        0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0xE0, 0xE7, 0x0F, 0xE0, 0xFF, 0x07, 0x00, 0x00, 0x00,  // Code for char )
        0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x3C, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char *
        0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char +
        0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ,
        0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char -
        0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char .
        0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x1E, 0x00, 0x80, 0x0F, 0x00, 0xE0, 0x03, 0x00, 0xF0, 0x00, 0x00, 0x0E, 0x00, 0x80, 0x0F, 0x00, 0xE0, 0x03, 0x00, 0xF8, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char /
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xE7, 0x1F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char 0
        0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char 1
        0x0B, 0x00, 0x00, 0x00, 0x08, 0xF0, 0x17, 0x18, 0xF0, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF0, 0x17, 0x10, 0xF8, 0x0F, 0x00, 0x00, 0x00, 0x00,  // Code for char 2
        0x0B, 0x00, 0x00, 0x00, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF0, 0xF7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char 3
        0x0B, 0x00, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0xE0, 0x17, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0xF0, 0xF7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char 4
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0x0F, 0x10, 0xF8, 0x17, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x08, 0xF0, 0x0F, 0x00, 0xF0, 0x1F, 0x00, 0x00, 0x00,  // Code for char 5
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xF7, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x08, 0xF0, 0x0F, 0x00, 0xF0, 0x1F, 0x00, 0x00, 0x00,  // Code for char 6
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0x0F, 0x00, 0xF8, 0x07, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char 7
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xF7, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF0, 0xF7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char 8
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0x0F, 0x10, 0xF8, 0x17, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF0, 0xF7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char 9
        0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x81, 0x01, 0x80, 0x81, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char :
        0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0xF8, 0x80, 0x01, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ;
        0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x66, 0x00, 0x00, 0xC3, 0x00, 0x80, 0x81, 0x01, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char <
        0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x66, 0x00, 0x00, 0x66, 0x00, 0x00, 0x66, 0x00, 0x00, 0x66, 0x00, 0x00, 0x66, 0x00, 0x00, 0x66, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char =
        0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x81, 0x01, 0x80, 0xC3, 0x01, 0x00, 0xE7, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char >
        0x0B, 0x00, 0x00, 0x00, 0x08, 0xF0, 0x19, 0x18, 0xF0, 0x19, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0xF0, 0x17, 0x00, 0xF8, 0x0F, 0x00, 0x00, 0x00, 0x00,  // Code for char ?
        0x0B, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0x17, 0xF8, 0xE7, 0x1F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x98, 0x7E, 0x19, 0x98, 0xE7, 0x19, 0x98, 0x81, 0x19, 0x98, 0x81, 0x19, 0xF0, 0xE7, 0x01, 0xF8, 0xFF, 0x01, 0x00, 0x00, 0x00,  // Code for char @
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x1F, 0xF8, 0xF7, 0x0F, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0xF0, 0xF7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char A
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xF7, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xE0, 0xF7, 0x0F, 0xE0, 0xFF, 0x07, 0x00, 0x00, 0x00,  // Code for char B
        0x0A, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xE7, 0x1F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char C
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xE7, 0x1F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0xE0, 0xE7, 0x0F, 0xE0, 0xFF, 0x07, 0x00, 0x00, 0x00,  // Code for char D
        0x0A, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xFF, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char E
        0x0A, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x1F, 0xF8, 0xF7, 0x0F, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char F
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xE7, 0x1F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x08, 0xF0, 0x0F, 0x00, 0xF0, 0x1F, 0x00, 0x00, 0x00,  // Code for char G
        0x0B, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x1F, 0xF0, 0xF7, 0x0F, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0xF0, 0xF7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char H
        0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char I
        0x0B, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x17, 0x00, 0xE0, 0x1F, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char J
        0x0B, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0x1F, 0xF0, 0xF7, 0x0F, 0x00, 0x1B, 0x00, 0x80, 0x1B, 0x00, 0xC0, 0x19, 0x00, 0xE0, 0x18, 0x00, 0x70, 0x18, 0x00, 0x38, 0x18, 0x00, 0x18, 0xF0, 0x0F, 0x08, 0xF0, 0x1F, 0x00, 0x00, 0x00,  // Code for char K
        0x0B, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0x1F, 0xF0, 0xE7, 0x0F, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,  // Code for char L
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x1F, 0xF8, 0xE7, 0x0F, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0xD8, 0x0F, 0x00, 0xD8, 0x0F, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char M
        0x0B, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x1F, 0xE0, 0xEF, 0x0F, 0x40, 0x00, 0x00, 0xC0, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x80, 0x03, 0x00, 0x00, 0x07, 0x00, 0x00, 0x06, 0x00, 0xE0, 0xEF, 0x0F, 0xF0, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char N
        0x0B, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x07, 0xE0, 0xE7, 0x0F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0xE0, 0xE7, 0x0F, 0xE0, 0xFF, 0x07, 0x00, 0x00, 0x00,  // Code for char O
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x1F, 0xF8, 0xF7, 0x0F, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0xE0, 0x17, 0x00, 0xE0, 0x0F, 0x00, 0x00, 0x00, 0x00,  // Code for char P
        0x0C, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xE7, 0x1F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x13, 0x18, 0x00, 0x07, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x18,  // Code for char Q
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x1F, 0xF8, 0xF7, 0x0F, 0x18, 0xD8, 0x00, 0x18, 0xD8, 0x01, 0x18, 0x98, 0x03, 0x18, 0x18, 0x07, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x1C, 0xF0, 0x17, 0x18, 0xF8, 0x0F, 0x10, 0x00, 0x00, 0x00,  // Code for char R
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0x0F, 0x10, 0xF8, 0x17, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF0, 0x0F, 0x08, 0xF0, 0x1F, 0x00, 0x00, 0x00,  // Code for char S
        0x0B, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0xD8, 0xFF, 0x0F, 0xD8, 0xE7, 0x1F, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char T
        0x0B, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x17, 0xF0, 0xE7, 0x1F, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char U
        0x0B, 0x00, 0x00, 0x00, 0xF0, 0x7F, 0x00, 0xE0, 0xCF, 0x01, 0x00, 0x80, 0x03, 0x00, 0x00, 0x07, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x07, 0x00, 0x80, 0x03, 0xE0, 0xCF, 0x01, 0xF0, 0x7F, 0x00, 0x00, 0x00, 0x00,  // Code for char V
        0x0B, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0x17, 0xF0, 0xE7, 0x1F, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0xF0, 0x1F, 0x00, 0xF0, 0x1F, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char W
        0x0C, 0x00, 0x00, 0x00, 0x18, 0x00, 0x18, 0x78, 0x00, 0x1E, 0xF0, 0x81, 0x0F, 0xC0, 0xE7, 0x03, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xC0, 0xE7, 0x03, 0xF0, 0x81, 0x0F, 0x78, 0x00, 0x1E, 0x18, 0x00, 0x18,  // Code for char X
        0x0B, 0x00, 0x00, 0x00, 0xF0, 0x0F, 0x10, 0xE0, 0x17, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0xF0, 0xF7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char Y
        0x0B, 0x00, 0x00, 0x00, 0x08, 0x00, 0x18, 0x18, 0x00, 0x1E, 0x18, 0x80, 0x1F, 0x18, 0xE0, 0x1B, 0x18, 0xF0, 0x18, 0x18, 0x0E, 0x18, 0x98, 0x0F, 0x18, 0xE8, 0x03, 0x18, 0xF8, 0x00, 0x18, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char Z
        0x06, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xE7, 0x1F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char [
        0x0B, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x78, 0x00, 0x00, 0xF0, 0x01, 0x00, 0xC0, 0x07, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x70, 0x00, 0x00, 0xF0, 0x01, 0x00, 0xC0, 0x07, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00,  // Code for char BackSlash
        0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x10, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char ]
        0x0B, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x80, 0x01, 0x00, 0xC0, 0x00, 0x00, 0x70, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x70, 0x00, 0x00, 0xE0, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00,  // Code for char ^
        0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,  // Code for char _
        0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x18, 0x00, 0x00, 0x30, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char `
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x1F, 0xF8, 0xF7, 0x0F, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0xF0, 0xF7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char a
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xF7, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xE0, 0xF7, 0x0F, 0xE0, 0xFF, 0x07, 0x00, 0x00, 0x00,  // Code for char b
        0x0A, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xE7, 0x1F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char c
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xE7, 0x1F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0xE0, 0xE7, 0x0F, 0xE0, 0xFF, 0x07, 0x00, 0x00, 0x00,  // Code for char d
        0x0A, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xFF, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char e
        0x0A, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x1F, 0xF8, 0xF7, 0x0F, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char f
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xE7, 0x1F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x08, 0xF0, 0x0F, 0x00, 0xF0, 0x1F, 0x00, 0x00, 0x00,  // Code for char g
        0x0B, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x1F, 0xF0, 0xF7, 0x0F, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0xF0, 0xF7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char h
        0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char i
        0x0B, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x17, 0x00, 0xE0, 0x1F, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char j
        0x0B, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0x1F, 0xF0, 0xF7, 0x0F, 0x00, 0x1B, 0x00, 0x80, 0x1B, 0x00, 0xC0, 0x19, 0x00, 0xE0, 0x18, 0x00, 0x70, 0x18, 0x00, 0x38, 0x18, 0x00, 0x18, 0xF0, 0x0F, 0x08, 0xF0, 0x1F, 0x00, 0x00, 0x00,  // Code for char k
        0x0B, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0x1F, 0xF0, 0xE7, 0x0F, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,  // Code for char l
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x1F, 0xF8, 0xE7, 0x0F, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0xD8, 0x0F, 0x00, 0xD8, 0x0F, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char m
        0x0B, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x1F, 0xE0, 0xEF, 0x0F, 0x40, 0x00, 0x00, 0xC0, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x80, 0x03, 0x00, 0x00, 0x07, 0x00, 0x00, 0x06, 0x00, 0xE0, 0xEF, 0x0F, 0xF0, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char n
        0x0B, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x07, 0xE0, 0xE7, 0x0F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0xE0, 0xE7, 0x0F, 0xE0, 0xFF, 0x07, 0x00, 0x00, 0x00,  // Code for char o
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x1F, 0xF8, 0xF7, 0x0F, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0xE0, 0x17, 0x00, 0xE0, 0x0F, 0x00, 0x00, 0x00, 0x00,  // Code for char p
        0x0C, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x17, 0xF8, 0xE7, 0x1F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x13, 0x18, 0x00, 0x07, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x18,  // Code for char q
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0xFF, 0x1F, 0xF8, 0xF7, 0x0F, 0x18, 0xD8, 0x00, 0x18, 0xD8, 0x01, 0x18, 0x98, 0x03, 0x18, 0x18, 0x07, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x1C, 0xF0, 0x17, 0x18, 0xF8, 0x0F, 0x10, 0x00, 0x00, 0x00,  // Code for char r
        0x0B, 0x00, 0x00, 0x00, 0xE8, 0x0F, 0x10, 0xF8, 0x17, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xF0, 0x0F, 0x08, 0xF0, 0x1F, 0x00, 0x00, 0x00,  // Code for char s
        0x0B, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0xD8, 0xFF, 0x0F, 0xD8, 0xE7, 0x1F, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char t
        0x0B, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x17, 0xF0, 0xE7, 0x1F, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char u
        0x0B, 0x00, 0x00, 0x00, 0xF0, 0x7F, 0x00, 0xE0, 0xCF, 0x01, 0x00, 0x80, 0x03, 0x00, 0x00, 0x07, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x07, 0x00, 0x80, 0x03, 0xE0, 0xCF, 0x01, 0xF0, 0x7F, 0x00, 0x00, 0x00, 0x00,  // Code for char v
        0x0B, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0x17, 0xF0, 0xE7, 0x1F, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0xF0, 0x1F, 0x00, 0xF0, 0x1F, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0xF0, 0xE7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char w
        0x0C, 0x00, 0x00, 0x00, 0x18, 0x00, 0x18, 0x78, 0x00, 0x1E, 0xF0, 0x81, 0x0F, 0xC0, 0xE7, 0x03, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xC0, 0xE7, 0x03, 0xF0, 0x81, 0x0F, 0x78, 0x00, 0x1E, 0x18, 0x00, 0x18,  // Code for char x
        0x0B, 0x00, 0x00, 0x00, 0xF0, 0x0F, 0x10, 0xE0, 0x17, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0xF0, 0xF7, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00,  // Code for char y
        0x0B, 0x00, 0x00, 0x00, 0x08, 0x00, 0x18, 0x18, 0x00, 0x1E, 0x18, 0x80, 0x1F, 0x18, 0xE0, 0x1B, 0x18, 0xF0, 0x18, 0x18, 0x0E, 0x18, 0x98, 0x0F, 0x18, 0xE8, 0x03, 0x18, 0xF8, 0x00, 0x18, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char z
        0x09, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0xF8, 0xE7, 0x1F, 0xF8, 0xFF, 0x1F, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char {
        0x03, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x0F, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char |
        0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x10, 0x18, 0x00, 0x18, 0x18, 0x00, 0x18, 0xF8, 0xFF, 0x1F, 0xE8, 0xF7, 0x17, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char }
        0x0B, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x06, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x06, 0x00, 0x00, 0x06, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x06, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00,  // Code for char ~
        0x06, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x0F, 0x10, 0x00, 0x08, 0x10, 0x00, 0x08, 0x10, 0x00, 0x08, 0xF0, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // Code for char 
        };

