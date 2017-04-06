#include "stdint.h"
#include "system.h"
#include "cmsis_os.h"

#include "core/serv_core.h"
#include "core/clock_serv.h"

#define RTC_INT_PRIORITY (6 * 16)
#define RTC_SEC_UPDATE  (10)

enum 
{
    ANALOG_CLOCK_SCREEN = 0,
    DIGITAL_CLOCK_SCREEN,
    STOPWATCH_SCREEN,
    MAX_CLOCK_SCREEN
};

static void _do_analog_clock(void);
static void _do_digi_clock(void);
static void _do_stopwatch(void);
static void _do_clock_screen(void);

static QueueHandle_t queue;
static uint8_t curr_screen = ANALOG_CLOCK_SCREEN;

static void (*sub_screen[MAX_CLOCK_SCREEN])(void) = \
{_do_analog_clock, _do_digi_clock, _do_stopwatch};

struct screen clock_screen = {_do_clock_screen, &queue, 0};

/*----------------------------------------------------------------------------*/
static void 
_do_clock_screen(void)
{
	clock_screen.is_run = 1;
    queue = xQueueCreate(10, sizeof(uint8_t));
    while (clock_screen.is_run)
    {
        sub_screen[curr_screen]();
    }
}

/*----------------------------------------------------------------------------*/
static void 
_rtc_event_handler(void)
{
    BaseType_t higher_priority_woken;
    uint8_t message = RTC_SEC_UPDATE;

    xQueueSendToBackFromISR(queue, &message, &higher_priority_woken); 
    portYIELD_FROM_ISR(higher_priority_woken);
}

static void
_clock_event_deinit(void)
{
    clock_rtc_remove_cb(_rtc_event_handler);
    clock_rtc_disable_interrupt();
}

static void
_clock_event_init(void)
{
	clock_rtc_add_cb(_rtc_event_handler);
	clock_rtc_enable_interrupt(RTC_INT_PRIORITY);
}

static void 
_do_analog_clock(void)
{
    uint8_t message = 0;

    analog_clock_init();
	_clock_event_init();

    while (1)
    {
        xQueueReceive(queue, &message, portMAX_DELAY);
        
        switch (message) 
        {
        case RTC_SEC_UPDATE:
            analog_clock_update();
            break;
        case BTN6_PRESSED:
            curr_screen = DIGITAL_CLOCK_SCREEN;
            return;
		case BTN6_LONG_PRESSED:
			clock_screen.is_run = 0;
			return;
        default:
            break;
        }
    }
}

static void 
_do_digi_clock(void)
{
    uint8_t message = 0;

    digi_clock_init();
	_clock_event_init();

    while (1)
    {
        xQueueReceive(queue, &message, portMAX_DELAY);
        
        switch (message) 
        {
        case RTC_SEC_UPDATE:
            digi_clock_update();
            break;
        case BTN6_PRESSED:
            curr_screen = STOPWATCH_SCREEN;
            _clock_event_deinit();
            return;
		case BTN6_LONG_PRESSED:
            _clock_event_deinit();
			clock_screen.is_run = 0;
			return;
        default:
            break;
        }
    }
}

/*----------------------------------------------------------------------------*/
enum 
{
    STOPWATCH_START = 50,
    STOPWATCH_STOP,
};

static void
_do_stopwatch(void)
{
    uint8_t message = 0;
    uint8_t status = STOPWATCH_STOP;

    stopwatch_init();    
    
    while (1)
    {
        xQueueReceive(queue, &message, portMAX_DELAY);
        
        switch (message)
        {
        case BTN5_PRESSED:
            if (STOPWATCH_STOP == status)
            {
                stopwatch_start();
                status = STOPWATCH_START;
            }
            else if (STOPWATCH_START == status)
            {
                stopwatch_stop();
                status = STOPWATCH_STOP;
            }
            break;
        case BTN6_PRESSED:
            curr_screen = ANALOG_CLOCK_SCREEN;
            stopwatch_deinit();
            return;
		case BTN6_LONG_PRESSED:
            stopwatch_deinit();
			clock_screen.is_run = 0;
			return;
        default:
            break;
        }
    }
}

