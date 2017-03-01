#include "stdint.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/cm3/systick.h"
#include "cmsis_os.h"
#include "system.h"

static void thread1(void *arg); 
static void thread2(void *arg); 
static void gpio_setup(void);

SemaphoreHandle_t mutex;

int main(void)
{
	system_init();

	gpio_setup();

	mutex = xSemaphoreCreateMutex();

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
	static int count1 = 0;
	
	while (1)
	{
		xSemaphoreTake(mutex, portMAX_DELAY);
		printf("%s : free heap=%d\r\n", __FUNCTION__,  xPortGetFreeHeapSize());
		//printf("hello-world\r\n");
		led_b2_toggle();
		xSemaphoreGive(mutex);
		vTaskDelay(200);
	}
}

static void thread2(void *arg) 
{
	(void)arg;
	static int count2 = 0;
	
	while (1)
	{
		xSemaphoreTake(mutex, portMAX_DELAY);
		printf("%s : count=%d\r\n", __FUNCTION__, count2++);
		gpio_toggle(GPIOA, GPIO0);
		xSemaphoreGive(mutex);
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

