#include "stdint.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/cm3/systick.h"
#include "cmsis_os.h"
#include "system.h"
#include "core/simple_lock.h"
#include "stdio.h"

static void thread1(void *arg); 
static void thread2(void *arg); 
static void gpio_setup(void);

DEFINE_LOCK(key);

int main(void)
{
	system_init();

	gpio_setup();

	xTaskCreate(thread1, "thread1", 128, NULL, 1, NULL);
	xTaskCreate(thread2, "thread2", 128, NULL, 1, NULL);
	
	vTaskStartScheduler();

	while (1)
	{
	}
}

static void thread1(void *arg)
{
	(void)arg;
	
	while (1)
	{
		LOCK(key);
		printf("%s line %d\r\n", __FUNCTION__, __LINE__);
		led_b2_toggle();
		UNLOCK(key);
		vTaskDelay(200);
	}
}

static void thread2(void *arg) 
{
	(void)arg;
	
	while (1)
	{
		LOCK(key);
		printf("%s line %d\r\n", __FUNCTION__, __LINE__);
		gpio_toggle(GPIOA, GPIO0);
		UNLOCK(key);
		vTaskDelay(200);
	}
}

static void gpio_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);

	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO2);
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);
}

