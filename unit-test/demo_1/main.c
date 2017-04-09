#include "stdint.h"
#include "system.h"
#include "stdio.h"
#include "cmsis_os.h"

#include "core/clock_serv.h"
#include "core/serv_core.h"
#include "core/button.h"

#define START_EVENT   (20)
#define STOP_EVENT   (21)

static void master_thread(void *arg);

uint8_t curr_screen = CLOCK_SCREEN;
extern struct screen clock_screen;
extern struct screen act_screen;
extern struct screen heart_screen;
struct screen *pscreen[MAX_SCREEN] = {&clock_screen, &act_screen, &heart_screen};

int main(void)
{
    serv_init();
    
    xTaskCreate(master_thread, "master_thread", 128, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1)
    {
    }
}

static void btn6_event_handler(uint8_t event)
{
    BaseType_t higher_priority_woken;
    uint8_t message = 0xFF;

    if (BTN_PRESSED == event)
    {
        message = BTN6_PRESSED;
		xQueueSendToBackFromISR(*(pscreen[curr_screen]->pqueue), &message, 
				                &higher_priority_woken); 
    }
    else
    {
        message = BTN6_LONG_PRESSED;
		xQueueSendToBackFromISR(*(pscreen[curr_screen]->pqueue), &message, 
				                &higher_priority_woken); 
		curr_screen++;
		if (MAX_SCREEN == curr_screen)
			curr_screen = CLOCK_SCREEN;
    }

    portYIELD_FROM_ISR(higher_priority_woken);
}

static void btn5_event_handler(uint8_t event)
{
    BaseType_t higher_priority_woken;
    uint8_t message = 0xFF;

    if (BTN_PRESSED == event)
    {
        message = BTN5_PRESSED;
    }
    else
    {
        return;
    }

    xQueueSendToBackFromISR(*(pscreen[curr_screen]->pqueue), &message, &higher_priority_woken); 
    portYIELD_FROM_ISR(higher_priority_woken);
}


static void master_thread(void *arg)
{
    (void)arg;

    button_register_event(BTN5, btn5_event_handler);
    button_register_event(BTN6, btn6_event_handler);
    button_enable(BTN5);
    button_enable(BTN6);

    while (1)
    {
        pscreen[curr_screen]->pfunc();
    }
}
