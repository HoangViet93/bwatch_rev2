#ifndef SERV_CORE_H
#define SERV_CORE_H

#include "cmsis_os.h"

struct screen 
{
    void (*pfunc)(void);
    QueueHandle_t *pqueue;
};

enum
{
    BTN5_PRESSED = 20,
    BTN5_LONG_PRESSED,
    BTN6_PRESSED,
    BTN6_LONG_PRESSED,
};

enum
{
    CLOCK_SCREEN = 0,
    MAX_SCREEN,
};

extern const struct ili9163 lcd_conf;   /* TODO remove it */
extern const struct adxl345 adxl345_conf;   /* TODO remove it */
void serv_init(void);

#endif

