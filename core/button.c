#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/cm3/nvic.h"
#include "libopencm3/stm32/exti.h"
#include "system.h"
#include "stddef.h"
#include "core/button.h"

#define BTN_PRESSED  (0)
#define BTN_RELEASE  (1)
#define BTN_LONG_TIMEOUT  (1000)
#define BTN_SHORT_TIMEOUT  (40)
#define BTN_EXTI_PRI (7 * 16)

/* debug purpose */
#if (0) || defined(CONFIG_ENABLE_DEBUG) 
#include "stdio.h"
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

struct button 
{
    uint32_t nvic;
    uint32_t exti;
    uint32_t port;
    uint16_t pin;
    uint8_t state;
    uint32_t count;
    void (*pfunc)(uint8_t event);
};

struct button btn[BTN_MAX] = 
{
    {NVIC_EXTI9_5_IRQ, EXTI5, GPIOB, GPIO5, BTN_RELEASE, 0, NULL},
    {NVIC_EXTI9_5_IRQ, EXTI6, GPIOB, GPIO6, BTN_RELEASE, 0, NULL},
};

static void 
_exti_conf(uint8_t id)
{
	rcc_periph_clock_enable(RCC_AFIO);
	gpio_set_mode(btn[id].port, GPIO_MODE_INPUT, 
                  GPIO_CNF_INPUT_FLOAT, btn[id].pin);

	/* Configure the EXTI subsystem. */
	exti_select_source(btn[id].exti, btn[id].port);
	exti_set_trigger(btn[id].exti, EXTI_TRIGGER_BOTH);
	exti_enable_request(btn[id].exti);
}

void 
button_init(uint8_t id)
{
    _exti_conf(id);
}

int8_t 
button_register_event(uint8_t id, void (*pfunc)(uint8_t event))
{
    if (NULL == btn[id].pfunc)
    {
        btn[id].pfunc = pfunc;
        return 0;
    }
    return -1;
}

int8_t 
button_unregister_event(uint8_t id, void (*pfunc)(uint8_t event))
{
    if (pfunc == btn[id].pfunc)
    {
        btn[id].pfunc = NULL;
        return 0;
    }
    return -1;
}

void 
button_enable(uint32_t id)
{
    nvic_set_priority(btn[id].nvic, BTN_EXTI_PRI);
    nvic_enable_irq(btn[id].nvic);
}

void 
button_disable(uint32_t id)
{
    nvic_disable_irq(btn[id].nvic);
}


static void
_button_handler(uint8_t id)
{
    if (exti_get_flag_status(btn[id].exti) != btn[id].exti)
    {
        return;
    }

    if (gpio_get(btn[id].port, btn[id].pin) != btn[id].pin)
    {
        btn[id].state = BTN_PRESSED;
        btn[id].count = get_tick();
    }
    if ((gpio_get(btn[id].port, btn[id].pin) == btn[id].pin) 
        && (BTN_PRESSED == btn[id].state))
    {
        btn[id].state = BTN_RELEASE;
        
        if ((get_tick() - btn[id].count) > BTN_LONG_TIMEOUT)
        {
            LOG("%s long button pressed\r\n", __FILE__);
            if (NULL != btn[id].pfunc)
            {
                btn[id].pfunc(BTN_LONG_PRESSED);
            }
        }
        else if ((get_tick() - btn[id].count) > BTN_SHORT_TIMEOUT)
        {
            LOG("%s short button pressed\r\n", __FILE__);
            if (NULL != btn[id].pfunc)
            {
                btn[id].pfunc(BTN_PRESSED);
            }
        }
        LOG("%s btn[%d] diff=%d\r\n", __FILE__, id, get_tick() - btn[id].count);
        
        btn[id].count = get_tick();
    }
    
    exti_reset_request(btn[id].exti);
}

void exti9_5_isr(void)
{
    _button_handler(BTN5);
    _button_handler(BTN6);
}

