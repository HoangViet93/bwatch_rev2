#include "stdint.h"
#include "system.h"
#include "stdio.h"
#include "cmsis_os.h"

#include "core/clock_serv.h"
#include "core/serv_core.h"
#include "core/button.h"

#define START_EVENT   (20)
#define STOP_EVENT   (21)

static void main_thread(void *arg);
static void btn_handler(uint8_t event);

QueueHandle_t queue;

int main(void)
{
    serv_init();
    
    xTaskCreate(main_thread, "main_thread", 128, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1)
    {
    }
}

static void main_thread(void *arg)
{
    (void)arg;
    uint8_t mes = 0;
    
    queue = xQueueCreate(10, sizeof(uint8_t));
    
    button_register_event(BTN5, btn_handler);
    button_enable(BTN5);
    stopwatch_init();    

    while(1)
    {
        xQueueReceive(queue, &mes, portMAX_DELAY);
        switch(mes) {
        case START_EVENT:
            stopwatch_start();
            break;
        case STOP_EVENT:
            stopwatch_stop();
            break;
        default:
            break;
        }
    }
}

static void btn_handler(uint8_t event)
{
    static uint8_t flag = 0;
    uint8_t mes = 0;
    BaseType_t higher_priority_woken;

    if (BTN_PRESSED != event)
    {
        return;
    }

    if (0 == flag)
    {
        mes = START_EVENT;
        flag = 1;
    }
    else
    {
        mes = STOP_EVENT;
        flag = 0;
    }

    xQueueSendToBackFromISR(queue, &mes, &higher_priority_woken); 
    portYIELD_FROM_ISR(higher_priority_woken);
}

