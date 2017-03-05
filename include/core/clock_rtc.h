#ifndef CLOCK_RTC_H
#define CLOCK_RTC_H

#include "stdint.h"
#include "libopencm3/stm32/rcc.h"

struct time
{
	uint8_t hour;
	uint8_t minutes;
	uint8_t second;
};

int8_t clock_rtc_init(enum rcc_osc osc);
int8_t clock_rtc_set_time(struct time t);
void clock_rtc_get_time(struct time *t);

#endif

