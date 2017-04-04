#include "stdint.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/cm3/systick.h"
#include "cmsis_os.h"
#include "system.h"
#include "core/clock_rtc.h"
#include "libopencm3/stm32/rtc.h"
#include "stdio.h"

static void thread1(void *arg); 
static void thread2(void *arg); 
static void gpio_setup(void);
static void rtc_cb(void);

QueueHandle_t thread1_queue;

int main(void)
{
	system_init();

	gpio_setup();
	clock_rtc_init(RCC_LSI);

	xTaskCreate(thread1, "thread1", 128, NULL, 1, NULL);
	
	vTaskStartScheduler();

	while (1)
	{
	}
}

static void thread1(void *arg)
{
	(void)arg;
    uint32_t val = 0;

    printf("%s : start\r\n", __FUNCTION__);

    thread1_queue = xQueueCreate(10, sizeof(uint32_t));
	clock_rtc_add_cb(rtc_cb);
	clock_rtc_enable_interrupt(16 * 5);
	
	while (1)
	{
        xQueueReceive(thread1_queue, &val, portMAX_DELAY); 
        printf("%s : receive message, val = %d\r\n", __FUNCTION__, val);
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

static void rtc_cb(void)
{
    BaseType_t higher_priority_woken;
    static uint32_t val = 5;

    led_b2_toggle();
    printf("interrupt called send message to thread1_queue \r\n");
    xQueueSendToBackFromISR(thread1_queue, &val, 
                             &higher_priority_woken); 
    portYIELD_FROM_ISR(higher_priority_woken);
    val++;
}

