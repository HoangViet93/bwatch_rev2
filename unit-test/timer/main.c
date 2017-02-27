#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/timer.h"
#include "libopencm3/cm3/nvic.h"
#include "system.h"

void timer_setup(void);
void clock_setup(void);
void gpio_setup(void);

int main(void)
{
	system_init();

	clock_setup();
	gpio_setup();
	timer_setup();

	while (1)
	{
	}
}
	
void timer_setup(void)
{
	rcc_periph_reset_pulse(RST_TIM2);
	
	nvic_enable_irq(NVIC_TIM2_IRQ);

	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, 
				   TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_prescaler(TIM2, ((rcc_apb1_frequency * 2) / 10000) - 1);

	timer_disable_preload(TIM2);
	timer_continuous_mode(TIM2);
	timer_set_period(TIM2, 10000);
	timer_enable_counter(TIM2);
	timer_enable_irq(TIM2, TIM_DIER_UIE);
}

void clock_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_TIM2);
}

void gpio_setup(void)
{
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,		
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO2);		
}

void tim2_isr(void)
{
	if (1 == timer_get_flag(TIM2, TIM_SR_UIF)){
		timer_clear_flag(TIM2, TIM_SR_UIF);

		gpio_toggle(GPIOB, GPIO2);
	}
}

