#include "stdint.h"
#include "system.h"
#include "stdio.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/spi.h"
#include "core/ili9163.h"
#include "core/graphics.h"
#include "core/clock_rtc.h"

static void clock_setup(void);
static void get_date_str(char *pbuf);

static const struct font digital_12x24= 
{
	.width = 12,
	.height = 24,
	.data = Digital_Mono12x24,
	.text_color = WHITE,
	.bkg_color = BLACK
};

static const struct font mono7x13= 
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

struct time t;
uint8_t clock_str_buf[10];

int main(void)
{
	uint16_t str_width = 0;
    uint16_t str_height = 0;
    uint16_t time_x, time_y;
    uint16_t date_x, date_y;
    char date_buf[10] = {0};

	system_init();
	clock_setup();
	clock_rtc_init(RCC_LSI);

	ili9163_init(&ili_conf);
	ili9163_set_screen(&ili_conf, 0x0000);
    
    get_date_str(date_buf);
    str_width = font_get_str_width(&digital_12x24, "00:00:00");
    str_height = font_get_str_height(ili_conf.lcd_x_size, &digital_12x24, "00:00:00");

    time_y = ili_conf.lcd_y_size/2 - str_height/2;
    time_x = ili_conf.lcd_x_size/2 - str_width/2;

    str_width = font_get_str_width(&mono7x13, "00 Mar 0000");
    str_height = font_get_str_height(ili_conf.lcd_x_size, &mono7x13,"00 Mar 0000");
    date_y = ili_conf.lcd_y_size/2 - str_height/2;
    date_x = ili_conf.lcd_x_size/2 - str_width/2;
    date_y += digital_12x24.height/2 + 7;

    date_y -= 10;
    time_y -= 10;

	while (1)
	{
		clock_rtc_get_time(&t);
		sprintf(clock_str_buf, "%d:%d:%d", t.hour, t.minutes, t.second);
		ili9163_print(&ili_conf, time_x - 1, time_y - 1, clock_str_buf, &digital_12x24);
		ili9163_print(&ili_conf, date_x, date_y, date_buf, &mono7x13);

        if (t.hour > 23)
        {
            memset(date_buf, 0, 10);
            get_date_str(date_buf);
        }

		delay_ms(980);
	}
}

static void get_date_str(char *pbuf)
{
    struct date d;

    const char month_str[12][4] = 
    {
        "Jan","Feb","Mar","Apr", "May", "Jun", "Jul", 
        "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    clock_rtc_get_date(&d);
    sprintf(pbuf, "%d %s %d", d.day, month_str[d.month - 1], d.year);
}

static void clock_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_SPI2);
}

