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

struct date
{
	uint8_t day;
	uint8_t month;
	uint16_t year;
};

int8_t clock_rtc_init(enum rcc_osc osc);

int8_t clock_rtc_set_time(struct time t);
void clock_rtc_get_time(struct time *t);

int8_t clock_rtc_add_cb(void (*cb)(void));
int8_t clock_rtc_remove_cb(void (*cb)(void));

void clock_rtc_enable_interrupt(uint8_t priority);
void clock_rtc_disable_interrupt(void);

void clock_rtc_get_date(struct date *d);
int8_t clock_rtc_set_date(struct date d);

#endif

