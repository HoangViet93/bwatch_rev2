#include "core/clock_rtc.h"
#include "libopencm3/stm32/rtc.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/pwr.h"
#include "libopencm3/cm3/nvic.h"
#include "libopencm3/stm32/f1/bkp.h"

#define CLOCK_BKP_REG BKP_DR1
#define TIME_ALREADY_SET    (0x2a)

static struct time _get_time_from_compiler(void);

int8_t
clock_rtc_init(enum rcc_osc osc)
{

	if (osc != RCC_LSI && osc != RCC_LSE)
	{
		return -1;
	}

	/* setup clock source for rtc */
	rcc_osc_on(osc);
	rcc_wait_for_osc_ready(osc);
	
	/* setup rtc config */
	if (RCC_LSI == osc)
	{
		rtc_auto_awake(RCC_LSI, 40000 - 1);
	}
	else 
	{
		rtc_auto_awake(RCC_LSI, 32768 -1);
	}
	
	/* set time */
	if (TIME_ALREADY_SET != CLOCK_BKP_REG)
	{
		clock_rtc_set_time(_get_time_from_compiler());	
	}
	else 
	{
		CLOCK_BKP_REG = TIME_ALREADY_SET;
	}

	return 0;
}

static uint8_t 
_conv2d(const char* p) 
{
	uint8_t v = 0;
	if ('0' <= *p && *p <= '9') 
	{
		v = *p - '0';
	}
	return 10 * v + *++p - '0';
}

static struct time 
_get_time_from_compiler(void)
{
	struct time t;

	t.hour = _conv2d(__TIME__);
	t.minutes = _conv2d(__TIME__ + 3);
	t.second = _conv2d(__TIME__ + 6);

	return t;
}

void
clock_rtc_enable_interrupt(void)
{
	nvic_enable_irq(NVIC_RTC_IRQ);
	nvic_set_priority(NVIC_RTC_IRQ, 1);
	rtc_interrupt_enable(RTC_SEC);
}

int8_t 
clock_rtc_set_time(struct time t)
{
	volatile uint32_t sec = 0;

	if (t.hour >= 24 || t.minutes >= 60 || t.second >= 60)
	{
		return -1;
	}

	sec = t.hour * 3600 + t.minutes * 60 + t.second;

	rtc_set_counter_val(sec);

	return 0;
}

void
clock_rtc_get_time(struct time *t)
{
	uint32_t sec = 0;

	sec = rtc_get_counter_val();

	t->hour = (uint8_t)(sec / 3600);
	t->minutes = (uint8_t)((sec % 3600) / 60);
	t->second = (uint8_t)((sec % 3600) % 60);
}

void rtc_isr(void)
{
	rtc_clear_flag(RTC_SEC);
}

