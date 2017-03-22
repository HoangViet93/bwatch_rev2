#include "stdint.h"
#include "system.h"
#include "stdio.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/spi.h"
#include "core/ili9163.h"
#include "core/graphics.h"
#include "core/clock_rtc.h"

#define NEW_BUTTON (100)

static void clock_setup(void);
static void get_date_str(char *pbuf);
static void gpio_setup(void);

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

QueueHandle_t queue;

int main(void)
{
	system_init();
	clock_setup();
	clock_rtc_init(RCC_LSI);

    queue = xQueueCreate(10, sizeof(uint32_t));

	ili9163_init(&ili_conf);
	ili9163_set_screen(&ili_conf, 0x0000);
    
	while (1)
	{
		clock_rtc_get_time(&t);

		delay_ms(980);
	}
}
/*----------------------------------------------------------------------------*/
static void stop_watch(void *arg)
{
    struct time t = {0, 0, 0};
    uint8_t clock_str_buf[10];
	uint16_t str_width = 0;
    uint16_t str_height = 0;
    uint16_t time_x, time_y;

    (void)arg;
    str_width = font_get_str_width(&digital_12x24, "00:00:00");
    str_height = font_get_str_height(ili_conf.lcd_x_size, &digital_12x24, "00:00:00");

    time_y = ili_conf.lcd_y_size/2 - str_height/2;
    time_x = ili_conf.lcd_x_size/2 - str_width/2;

	sprintf(clock_str_buf, "%d:%d:%d", t.hour, t.minutes, t.second);
	ili9163_print(&ili_conf, time_x - 1, time_y - 1, clock_str_buf, &digital_12x24);

    while (1)
    {
        xQueueReceive(queue, &mes, 100); 

    }
}

/*----------------------------------------------------------------------------*/
static uint8_t btn_level[2] = {32, 64};
static uint8_t btn_oldlevel[2] = {32, 64};
static void scan_btn(void *arg)
{
    (void)arg;
    static uint32_t mes = NEW_BUTTON;

    gpio_setup();

    while (1)
    {
        btn_level[1] = gpio_get(GPIOB, GPIO6); 

        if (btn_level[1] != btn_oldlevel[1])
        {
            if (btn_level[1] != 0)
            {
                printf("button pressed sent event\r\n");
                xQueueSendToFront(queue, &mes, 100); 
            }
            btn_oldlevel[1] = btn_level[1];
        }
        vTaskDelay(10);
	}
}

/*----------------------------------------------------------------------------*/
static void gpio_setup(void)
{
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT,		
				  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO5);		
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT,		
				  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO6);		

	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO2);
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);
}

static void clock_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_SPI2);
}

