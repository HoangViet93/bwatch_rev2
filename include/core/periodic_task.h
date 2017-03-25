#ifndef PERIODIC_TASK_H
#define PERIODIC_TASK_H

#include "stdint.h"

enum {
    TASK_THREAD_CONTEX = 0,
    TASK_INTERRUPT_CONTEX = 2,
    TASK_EXECUTING = 4
};

struct periodic_task
{
    uint32_t interval;
    uint32_t remain;
    void (*pfunc)(void);
    uint8_t flag;
};

void periodic_task_init(uint8_t priority);
int8_t periodic_task_register(void (*pfunc)(void), uint16_t interval, uint8_t flag);
int8_t periodic_task_unregister(void (*pfunc)(void));

#endif

