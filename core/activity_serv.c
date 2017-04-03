#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/i2c.h"
#include "libopencm3/cm3/nvic.h"
#include "libopencm3/stm32/exti.h"
#include "core/adxl345.h"
#include "core/graphics.h"
#include "core/activity_serv.h"
#include "core/ili9163.h"
#include "core/serv_core.h"

/* debug purpose */
#if (0) || defined(CONFIG_ENABLE_DEBUG) 
#include "stdio.h"
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#define ACTIVITY_INT_PRIORITY	(7 * 16)

static void _activity_serv_init_gyro(void);
static void _activity_serv_init_exti(void);

static void (*event_pfunc)(uint8_t event) = NULL;

void 
activity_serv_init(void)
{
	LCD_LOCK();
    ili9163_set_screen(&lcd_conf, BLACK);
    LCD_UNLOCK();

	_activity_serv_init_exti();
	_activity_serv_init_gyro();
}

int8_t
activity_serv_event_cb_add(void (*pfunc)(uint8_t event))
{
	if (NULL != event_pfunc)
	{
		return -1;
	}
	else
	{
		event_pfunc = pfunc;
		return 0;
	}
}

int8_t
activity_serv_event_cb_remove(void (*pfunc)(uint8_t event))
{
	if (pfunc != event_pfunc)
	{
		return -1;
	}
	else
	{
		event_pfunc = NULL;
		return 0;
	}
}

void
activity_serv_event_enable(void)
{
    nvic_set_priority(NVIC_EXTI1_IRQ, ACTIVITY_INT_PRIORITY);
    nvic_enable_irq(NVIC_EXTI1_IRQ);
}

void
activity_serv_event_disable(void)
{
    nvic_disable_irq(NVIC_EXTI1_IRQ);
}

void
activity_serv_deinit(void)
{
	event_pfunc = NULL;
	activity_serv_event_disable();
}

static void 
_activity_serv_init_gyro(void)
{
	__attribute__((unused)) uint8_t int_source = 0;

	adxl345_init(&adxl345_conf);
	
	adxl345_write_byte(adxl345_conf.i2c, ADXL345_POWER_CTL, 0x28);
	adxl345_range_set(&adxl345_conf, ADXL345_RANGE_16G);
	adxl345_write_byte(adxl345_conf.i2c, ADXL345_BW_RATE, ADXL345_RATE_100HZ);	

	adxl345_activity_thresh_set(&adxl345_conf, 2000);
	adxl345_activity_ctl(&adxl345_conf, ADXL345_ACT_X | ADXL345_ACT_Y | ADXL345_ACT_Z);

	adxl345_inactivity_thresh_set(&adxl345_conf, 185);
	adxl345_inactivity_ctl(&adxl345_conf, ADXL345_INACT_ACDC | ADXL345_INACT_X  \
				          | ADXL345_INACT_Y | ADXL345_INACT_Z);
	adxl345_write_byte(adxl345_conf.i2c, ADXL345_TIME_INACT, 2);	

	adxl345_freefall_thresh_set(&adxl345_conf, 750);
	adxl345_freefall_time_set(&adxl345_conf, 30);

	adxl345_interrupt_set(&adxl345_conf, INT_PIN_1, ADXL345_ACTIVITY | ADXL345_INACTIVITY
						  | ADXL345_FREE_FALL);
	
	int_source = adxl345_read_byte(adxl345_conf.i2c, ADXL345_INT_SOURCE);
}

static void 
_activity_serv_init_exti(void)
{
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, 
                  GPIO_CNF_INPUT_FLOAT, GPIO1);

	/* Configure the EXTI subsystem. */
	exti_select_source(EXTI1, GPIOB);
	exti_set_trigger(EXTI1, EXTI_TRIGGER_BOTH);
	exti_enable_request(EXTI1);

    nvic_set_priority(NVIC_EXTI1_IRQ, 1);
    nvic_enable_irq(NVIC_EXTI1_IRQ);
}

static uint8_t 
_activity_read_event(void)
{
	uint8_t int_src = 0;
	
	int_src =  adxl345_read_byte(adxl345_conf.i2c, ADXL345_INT_SOURCE);
	if ((int_src & ADXL345_ACTIVITY) == ADXL345_ACTIVITY)
	{
		return ACT_EVENT_ACTIVITY;
	}
	else if ((int_src & ADXL345_INACTIVITY) == ADXL345_INACTIVITY)
	{
		return ACT_EVENT_INACTIVITY;
	}
	else if ((int_src & ADXL345_FREE_FALL) == ADXL345_FREE_FALL)
	{
		return ACT_EVENT_FF;
	}
	else
	{
		return -1;
	}
}

void exti1_isr(void)
{
	uint8_t event = 0;

    if (exti_get_flag_status(EXTI1) != EXTI1)
    {
        return;
    }

	event = _activity_read_event();
	if ((event_pfunc != NULL) && (event > 0))
	{
		event_pfunc(event);
	}

	LOG("%s int source=%x\r\n", __FILE__, source);

    exti_reset_request(EXTI1);
}

