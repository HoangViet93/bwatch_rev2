#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include "core/adxl345.h"
#include "core/activity_serv.h"
#include "core/serv_core.h"
#include "core/button.h"

static void _event_handler(uint8_t event);
static void _do_activity(void);

QueueHandle_t queue;
struct screen act_screen = {_do_activity, &queue, 0};

static void _do_activity(void)
{
	uint8_t mess = 0;

	queue = xQueueCreate(10, sizeof(uint8_t));

	activity_serv_init();
	activity_serv_event_cb_add(_event_handler);
	activity_serv_event_enable();
	
	while (1)
	{
		xQueueReceive(queue, &mess, portMAX_DELAY);
		switch(mess)
		{
		case ACT_EVENT_INACTIVITY:
		case ACT_EVENT_ACTIVITY:
		case ACT_EVENT_FF:
			activity_serv_update_stt(mess);	
			break;
		case BTN6_LONG_PRESSED:		
			activity_serv_event_disable();
			return;
		default:
			break;
		}
	}
}

static void _event_handler(uint8_t event)
{
    BaseType_t higher_priority_woken;
    static uint32_t val;

	val = event;

    xQueueSendToBackFromISR(queue, &val, &higher_priority_woken); 
    portYIELD_FROM_ISR(higher_priority_woken);
}
