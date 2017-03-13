#include "core/clock_rtc.h"
#include "libopencm3/stm32/rtc.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/pwr.h"
#include "libopencm3/cm3/nvic.h"
#include "libopencm3/stm32/f1/bkp.h"
#include "stddef.h"
#include "string.h"
#include "core/simple_lock.h"
#include "stdio.h"

#define CLOCK_BKP_REG BKP_DR1
#define TIME_ALREADY_SET     (0x2a)
#define CLOCK_LSI_PRESCALER  (40000)
#define CLOCK_LSE_PRESCALER  (32768)

/* debug purpose */
#if (0) || defined(CONFIG_ENABLE_DEBUG) 
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

/* lock for curr_date, just for careful */
DEFINE_LOCK(rtc_key);

static struct time _get_time_from_compiler(void);
static struct date _get_date_from_compiler(void);
static void _update_date(uint16_t day_elapsed);

static void (*rtc_cb)(void) = NULL;
static struct date curr_date = {0};
static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

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
		rtc_auto_awake(RCC_LSI, CLOCK_LSI_PRESCALER - 1);
	}
	else 
	{
		rtc_auto_awake(RCC_LSI, CLOCK_LSE_PRESCALER -1);
	}
	
	LOG("%s CLOCK_BKP_REG = %d\r\n", __FILE__, CLOCK_BKP_REG);

	
	if (TIME_ALREADY_SET != CLOCK_BKP_REG)
	{
		clock_rtc_set_time(_get_time_from_compiler());	
		clock_rtc_set_date(_get_date_from_compiler());
		LOG("%s : backup reg doesn't set before, get date and time from compiler\r\n", 
				__FILE__);
		CLOCK_BKP_REG = TIME_ALREADY_SET;
	}
	else 
	{
		LOG("%s : time already setted, set date only\r\n", __FILE__);
		clock_rtc_set_date(_get_date_from_compiler());
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

static struct date
_get_date_from_compiler(void)
{
	struct date d;
	char month_str[5] = {0};
	
	sscanf(__DATE__, "%s %d %d", month_str, (int*)&d.day, (int*)&d.year);
	
	d.month = (strstr(month_names, month_str) - month_names) / 3 + 1;

	return d;
}

void
clock_rtc_enable_interrupt(uint8_t priority)
{
	nvic_enable_irq(NVIC_RTC_IRQ);
	nvic_set_priority(NVIC_RTC_IRQ, priority);
	rtc_interrupt_enable(RTC_SEC);
}

void
clock_rtc_disable_interrupt(void)
{
	nvic_disable_irq(NVIC_RTC_IRQ);
	rtc_interrupt_disable(RTC_SEC);
}

int8_t
clock_rtc_add_cb(void (*cb)(void))
{
	if (NULL == cb || rtc_cb != NULL)
	{
		return -1;
	}
	rtc_cb = cb;
	return 0;
}

int8_t
clock_rtc_remove_cb(void (*cb)(void))
{
	if (cb == rtc_cb)
	{
		rtc_cb = NULL;
		return 0;
	}
	else 
	{
		return -1;
	}
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
	uint32_t curr_sec = 0;
	uint32_t curr_hour = 0;
	uint16_t day_elapsed = 0;

	curr_sec = rtc_get_counter_val();

	curr_hour = (curr_sec / 3600);
	t->minutes = (uint8_t)((curr_sec % 3600) / 60);
	t->second = (uint8_t)((curr_sec % 3600) % 60);

	/* change value if current hour elapsed */
	if (curr_hour > 23)
	{
		day_elapsed = curr_hour / 24;
		t->hour = curr_hour % 24;
		curr_sec -= ((curr_hour/24) * 24 * 3600);
		rtc_set_counter_val(curr_sec);
		_update_date(day_elapsed);

		LOG("%s: current hour is %d\r\n", __FILE__, curr_hour);
		LOG("%s: day elapsed is %ld\r\n", __FILE__, day_elapsed);
	}
	else 
	{
		t->hour = curr_hour;
	}
}

static int8_t
_is_leap_year(uint16_t year)
{
	if ((year % 4) != 0)
		return -1;
	
	if ((year % 100) != 0)
		return 0;

	if ((year % 400) == 0)
		return 0;
	else
		return -1;
}

static void
_update_date(uint16_t day_elapsed)
{
	struct date d;
	uint16_t i = 0;

	clock_rtc_get_date(&d);
	
	for (i = 0; i < day_elapsed; i++)
	{
		if (d.month == 1 || d.month == 3 || d.month == 5 || d.month == 7
            || d.month == 8 || d.month == 10 || d.month == 12)
		{
			if (d.day < 31)
			{
				d.day++;
			}
			else
			{
				if (d.month != 12)
				{
					d.month++;
					d.day = 1;
				}
				else
				{
					d.year++;
					d.day = 1;
					d.month = 1;
				}
			}
		}
		else if (d.month == 4 || d.month == 6 || d.month == 9 || d.month == 11)
		{
			if (d.day < 30)
			{
				d.day++;
			}
			else
			{
				d.month++;
				d.day = 1;
			}
		}
		else if (2 == d.month)
		{
			if (d.day < 28)
			{
				d.day++;
			}
			else if (28 == d.day)
			{
				if (0 == _is_leap_year(d.year))
				{
					d.day++;
				}
				else
				{
					d.month++;
					d.day = 1;
				}
			}
			else if (29 == d.day)
			{
				d.month++;
				d.day = 1;
			}
		}
	}
	/* store updated date to database */
	curr_date = d;
}

int8_t 
clock_rtc_set_date(struct date d)
{
	uint32_t curr_sec = 0;
	uint32_t curr_hour = 0;

	if (d.day > 31 || d.month > 12 || d.year > 2020 || d.year < 1993)
	{
		return -1;
	}
	
	LOCK(rtc_key);

	/* save to our database */
	curr_date.day = d.day;
	curr_date.month= d.month;
	curr_date.year = d.year;

	/* change value of sec counter to fit new date */
	curr_sec = rtc_get_counter_val();
	curr_hour = curr_sec / 3600;
	if (curr_hour > 24)
	{
		curr_sec -= ((curr_hour/24) * 24 * 3600);
		rtc_set_counter_val(curr_sec);
	}

	UNLOCK(rtc_key);

	return 0;
}

void
clock_rtc_get_date(struct date *d)
{
	struct time t = {0};

	LOCK(rtc_key);

	/* updated elapsed time */
	clock_rtc_get_time(&t);

	*d = curr_date;

	UNLOCK(rtc_key);
}

void rtc_isr(void)
{
	rtc_clear_flag(RTC_SEC);
	if (rtc_cb != NULL)
	{
		rtc_cb();
	}
}

