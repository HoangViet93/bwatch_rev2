#ifndef CLOCK_SERV_H
#define CLOCK_SERV_H

#include "core/clock_rtc.h"

void clock_serv_init(enum rcc_osc osc);
void _analog_clock_draw_face(void);
void _analog_clock_calculate_and_draw_hand1(struct time t);
void _analog_clock_update_hand(struct time t);

#endif

