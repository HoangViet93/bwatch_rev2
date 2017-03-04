#ifndef SYSTEM_H
#define SYSTEM_H

void system_init(void);

#if defined(CONFIG_LED_B2)

#include "libopencm3/stm32/gpio.h"

static inline void led_b2_on(void)
{
	GPIOB_BSRR |= GPIO2;
}

static inline void led_b2_off(void)
{
	GPIOB_BRR |= GPIO2;
}

static inline void led_b2_toggle(void)
{
	GPIOB_ODR ^= GPIO2;
}
#endif

#if defined(CONFIG_DELAYMS)
void delay_ms(volatile uint32_t milis);
uint32_t get_tick(void);
#endif

#endif
