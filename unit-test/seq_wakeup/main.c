#include "stdint.h"
#include "stdio.h"
#include "cmsis_os.h"
#include "system.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/cm3/systick.h"

enum
{
    TRANS_STATE = 1
};

enum 
{
    FUNC1 = 0,
    FUNC2,
    FUNC3
};

static void thread1(void *arg); 
static void thread2(void *arg); 
static void gpio_setup(void);
static void sub_func1(void);
static void sub_func2(void);
static void sub_func3(void);

QueueHandle_t queue;
uint8_t curr_func = FUNC1;
static void (*pfunc[3])(void) = {sub_func1, sub_func2, sub_func3}; 
uint8_t btn_level[2] = {32, 64};
uint8_t btn_oldlevel[2] = {32, 64};

int main(void)
{
	system_init();

	gpio_setup();

    queue = xQueueCreate(10, sizeof(uint32_t));

	xTaskCreate(thread1, "thread1", 128, NULL, 1, NULL);
	xTaskCreate(thread2, "thread2", 128, NULL, 1, NULL);
	
	vTaskStartScheduler();

	while (1)
	{
	}
}

static void thread1(void *arg)
{
	(void)arg;
	static int count1 = 0;
	
	while (1)
	{
        pfunc[curr_func]();
	}
}

static void thread2(void *arg) 
{
	(void)arg;
	static uint32_t mes = TRANS_STATE;
	
	while (1)
	{   
        btn_level[1] = gpio_get(GPIOB, GPIO6); 

        if (btn_level[1] != btn_oldlevel[1])
        {
            if (btn_level[1] != 0)
            {
                printf("button pressed sent event\r\n");
                xQueueSendToFront(queue, &mes, 100); 
            }
            btn_oldlevel[1] = btn_level[1];
        }
        vTaskDelay(10);
	}
}

static void sub_func1(void)
{
    uint32_t mes = 0;
    
    while(1)
    {
        xQueueReceive(queue, &mes, 100); 
        if (TRANS_STATE == mes)
        {
            curr_func = FUNC2;
            break;
        }
        printf("%s: running\r\n", __FUNCTION__);
    }
}

static void sub_func2(void)
{  
    uint32_t mes = 0;
    
    while(1)
    {
        xQueueReceive(queue, &mes, 100); 
        if (TRANS_STATE == mes)
        {
            curr_func = FUNC3;
            break;
        }
        printf("%s: running\r\n", __FUNCTION__);
    }
}

static void sub_func3(void)
{
    uint32_t mes = 0;
    
    while(1)
    {
        xQueueReceive(queue, &mes, 100); 
        if (TRANS_STATE == mes)
        {
            curr_func = FUNC1;
            break;
        }
        printf("%s: running\r\n", __FUNCTION__);
    }
}

static void gpio_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);

	gpio_set_mode(GPIOB, GPIO_MODE_INPUT,		
				  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO5);		
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT,		
				  GPIO_CNF_INPUT_PULL_UPDOWN, GPIO6);		

	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO2);
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);
}

