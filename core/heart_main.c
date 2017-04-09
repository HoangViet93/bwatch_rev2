#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include "core/serv_core.h"
#include "core/button.h"
#include "core/heart_serv.h"

#define HEART_RATE_NEW  (120)

static void _event_handler(uint16_t bpm, uint16_t *ibi_rate, uint8_t ibi_len);
static void _do_heart_rate(void);

struct heart_rate_val
{
	uint16_t *ibi_rate;
	uint8_t ibi_len;
	uint16_t bpm;
};

static struct heart_rate_val val = 
{
	.ibi_rate = NULL, 
	.ibi_len = 0, 
	.bpm = 0
};

static QueueHandle_t queue;
struct screen heart_screen = {_do_heart_rate, &queue, 0};

static void _do_heart_rate(void)
{
	uint8_t mess = 0;

	queue = xQueueCreate(5, sizeof(uint8_t));
	heart_serv_add_event(_event_handler);
	heart_serv_init();
	
	while (1)
	{
		xQueueReceive(queue, &mess, portMAX_DELAY);
		switch(mess)
		{
		case HEART_RATE_NEW:
			heart_serv_update();
			break;
		case BTN6_LONG_PRESSED:		
			heart_serv_deinit();
			return;
		default:
			break;
		}
	}
}

static void _event_handler(uint16_t bpm, uint16_t *ibi_rate, uint8_t ibi_len)
{
    BaseType_t higher_priority_woken;
	uint8_t mess = HEART_RATE_NEW;

	val.bpm = bpm;
	val.ibi_len = ibi_len;
	val.ibi_rate = ibi_rate;

    xQueueSendToBackFromISR(queue, &mess, &higher_priority_woken); 
    portYIELD_FROM_ISR(higher_priority_woken);
}


