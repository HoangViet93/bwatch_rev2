#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include "system.h"
#include "core/button.h"
#include "stdio.h"

static void clock_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);
}

static void btn5_cb(uint8_t event)
{
    if (BTN_LONG_PRESSED == event)
    {
        printf("BTN5 long pressed\r\n");
    }
    else
    {
        printf("BTN5 pressed\r\n");
        led_b2_toggle();
    }
}

static void btn6_cb(uint8_t event)
{
    if (BTN_LONG_PRESSED == event)
    {
        printf("BTN6 long pressed\r\n");
    }
    else
    {
        printf("BTN6 pressed\r\n");
        led_b2_toggle();
    }
}

int main(void)
{
    system_init();
	clock_setup();

    button_init(BTN5);
    button_register_event(BTN5, btn5_cb);
    button_enable(BTN5);

    button_init(BTN6);
    button_register_event(BTN6, btn6_cb);
    button_enable(BTN6);

	while (1)
    {
        delay_ms(1000);
    }

	return 0;
}
