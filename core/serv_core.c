#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/spi.h"
#include "libopencm3/stm32/i2c.h"

#include "core/ili9163.h"
#include "core/clock_serv.h"
#include "core/periodic_task.h"
#include "core/button.h"
#include "core/adxl345.h"

#define PERIODIC_TASK_PRIORITY  (1)

static void _clock_setup(void);

const struct ili9163 lcd_conf = 
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

	/* if u plan to use another SPI, remember init AF func for gpio */
	.spi_reg = SPI2 
};

const struct adxl345 adxl345_conf =
{
	.i2c = I2C2,
	.scl_port = GPIOB,
	.scl_pin = GPIO10,
	.sda_port = GPIOB,
	.sda_pin = GPIO11
};

void serv_init(void)
{
	/* common hardware */
	system_init();
	_clock_setup();
	ili9163_init(&lcd_conf);

	/* service module init */
	clock_serv_init(RCC_LSI);

    /* init periodic thread */
    periodic_task_init(PERIODIC_TASK_PRIORITY);

    /* init button */
    button_init(BTN5);
    button_init(BTN6);
}

static void _clock_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_ADC1);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_SPI2);
    rcc_periph_clock_enable(RCC_I2C2);
    rcc_periph_clock_enable(RCC_AFIO);
}

