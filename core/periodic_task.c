#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/timer.h"
#include "libopencm3/cm3/nvic.h"
#include "system.h"
#include "cmsis_os.h"
#include "core/periodic_task.h"
#include "string.h"

/* debug purpose */
#if (0) || defined(CONFIG_ENABLE_DEBUG) 
#include "stdio.h"
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#define POOL_SIZE   (5)
#define PERIODIC_TASK_QUEUE_SIZE (10)
#define TASK_HANDLER_STACK_SIZE  (128)
#define TIMER_PERIOD (10) /* x0.1ms */

#define TASK_POOL_LOCK() xSemaphoreTake(task_pool_key, 100)
#define TASK_POOL_UNLOCK() xSemaphoreGive(task_pool_key)

static void _periodic_task_handler(void *arg);
static void _timer_init(void);
static void _timer_enable_interrupt(void);
static void _timer_disable_interrupt(void);

struct periodic_task task_pool[POOL_SIZE] = {0};
QueueHandle_t task_queue;
SemaphoreHandle_t task_pool_key;

/*----------------------------------------------------------------------------*/
void
periodic_task_init(uint8_t priority)
{
    memset((void*)task_pool, 0, sizeof(struct periodic_task)*POOL_SIZE);
    xTaskCreate(_periodic_task_handler, "periodic_task_handler", TASK_HANDLER_STACK_SIZE, 
                NULL, priority, NULL);
}

int8_t 
periodic_task_register(void (*pfunc)(void), uint16_t interval, uint8_t flag)
{
    uint8_t i = 0;

    for (i = 0; i < POOL_SIZE; i++) 
    {
        if (NULL == task_pool[i].pfunc)   
        {
            task_pool[i].pfunc = pfunc;
            task_pool[i].interval = interval - 1;
            task_pool[i].flag = 0;
            task_pool[i].flag |= flag;
            task_pool[i].remain = interval;
            return 0;
        }
    }
    return -1;
}

int8_t 
periodic_task_unregister(void (*pfunc)(void))
{
    uint8_t i = 0;

    for (i = 0; i < POOL_SIZE; i++) 
    {
        if (pfunc == task_pool[i].pfunc)   
        {
            task_pool[i].pfunc = NULL;
            memset(&task_pool[i], 0, sizeof(struct periodic_task));
            return 0;
        }
    }
    return -1;
}

static void 
_periodic_task_handler(void *arg)
{
    (void)arg;
    uint8_t index = 0;

    task_queue = xQueueCreate(PERIODIC_TASK_QUEUE_SIZE, sizeof(uint8_t));
	task_pool_key = xSemaphoreCreateMutex();

    _timer_init();
    _timer_enable_interrupt();

    while(1)
    {
        xQueueReceive(task_queue, &index, portMAX_DELAY);

        TASK_POOL_LOCK();
        if (task_pool[index].pfunc != NULL && 
            ((task_pool[index].flag & TASK_EXECUTING) == TASK_EXECUTING))
        {
            LOG("%s: execute in thread contex\r\n", __FUNCTION__);
            task_pool[index].pfunc();
            task_pool[index].flag &= ~TASK_EXECUTING;
        }
        TASK_POOL_UNLOCK();
    }
}

static void
_timer_init(void)
{
    rcc_periph_clock_enable(RCC_TIM2);
	rcc_periph_reset_pulse(RST_TIM2);
	
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, 
				   TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_prescaler(TIM2, ((rcc_apb1_frequency * 2) / 10000) - 1);

	timer_disable_preload(TIM2);
	timer_continuous_mode(TIM2);
	timer_set_period(TIM2, TIMER_PERIOD - 1);
	timer_enable_counter(TIM2);
}

static void
_timer_enable_interrupt(void)
{
    nvic_set_priority(NVIC_TIM2_IRQ, 16 * 6);
	nvic_enable_irq(NVIC_TIM2_IRQ);
	timer_enable_irq(TIM2, TIM_DIER_UIE);
}

static void
_timer_disable_interrupt(void)
{
	nvic_disable_irq(NVIC_TIM2_IRQ);
	timer_disable_irq(TIM2, TIM_DIER_UIE);
}

static void 
_check_task_pool(void)
{
    BaseType_t higher_priority_woken;
    uint8_t i = 0;

    for (i = 0; i < POOL_SIZE; i++) 
    {
        if (NULL == task_pool[i].pfunc || 
           ((task_pool[i].flag & TASK_EXECUTING) == TASK_EXECUTING))
        {
            continue;
        }

        if (task_pool[i].remain == 0)
        {
            if ((task_pool[i].flag & TASK_INTERRUPT_CONTEX) == TASK_INTERRUPT_CONTEX)
            {
                task_pool[i].pfunc();
                LOG("%s: execute in interrupt contex\r\n", __FUNCTION__);
            }
            else
            {
                xQueueSendToBackFromISR(task_queue, &i, &higher_priority_woken);
                task_pool[i].flag |= TASK_EXECUTING;
            }
            task_pool[i].remain = task_pool[i].interval;
        }
        else
        {
            task_pool[i].remain--;
        }
    }
}

void tim2_isr(void)
{
	if (1 == timer_get_flag(TIM2, TIM_SR_UIF))
    {
		timer_clear_flag(TIM2, TIM_SR_UIF);
        _check_task_pool();    
	}
}

