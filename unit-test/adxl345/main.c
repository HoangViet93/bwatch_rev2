#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "core/adxl345.h"
#include "core/activity_serv.h"
#include "core/serv_core.h"
#include "core/button.h"

static void event_handler(uint8_t event);

QueueHandle_t queue;

static void master_thread(void *args)
{
	(void)args;
	uint8_t mess = 0;

	queue = xQueueCreate(20, sizeof(uint8_t));

	activity_serv_init();
	activity_serv_event_cb_add(event_handler);
	activity_serv_event_enable();
	
	while (1)
	{
		xQueueReceive(queue, &mess, portMAX_DELAY);
		activity_serv_update_stt(mess);	
	}
}

int main(void)
{
	serv_init();
	
	xTaskCreate(master_thread, "master_thread", 128, NULL, 1, NULL);
    vTaskStartScheduler();

	while (1)
	{
	}
}

static void event_handler(uint8_t event)
{
    BaseType_t higher_priority_woken;
    static uint32_t val;

	val = event;

    xQueueSendToBackFromISR(queue, &val, &higher_priority_woken); 
    portYIELD_FROM_ISR(higher_priority_woken);
}
