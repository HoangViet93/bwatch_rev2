#ifndef CLOCK_SERV_H
#define CLOCK_SERV_H

#include "core/clock_rtc.h"

void clock_serv_init(enum rcc_osc osc);

void digi_clock_init(void);
void digi_clock_update(void);
void digi_clock_deinit(void);

void analog_clock_init(void);
void analog_clock_update(void);
void analog_clock_deinit(void);

#endif

