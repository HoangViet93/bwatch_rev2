#include "stdint.h"
#include "stdio.h"
#include "cmsis_os.h"
#include "system.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/cm3/systick.h"
#include "core/periodic_task.h"

static void sub_func1(void);
static void sub_func2(void);
static void sub_func3(void);
static void sub_func4(void);
static void sub_func5(void);
static void gpio_setup(void);
static void print_delay(void *arg);

uint32_t delaytime[5] = {0};

#define EXECUTE_CONTEX  TASK_THREAD_CONTEX

int main(void)
{
	system_init();

	gpio_setup();
    periodic_task_init();
    periodic_task_register(sub_func1, 200, EXECUTE_CONTEX);
    periodic_task_register(sub_func2, 200, EXECUTE_CONTEX);
    periodic_task_register(sub_func3, 200, EXECUTE_CONTEX);
    periodic_task_register(sub_func4, 200, EXECUTE_CONTEX);
    periodic_task_register(sub_func5, 200, EXECUTE_CONTEX);
    if (periodic_task_register(sub_func5, 600, EXECUTE_CONTEX) < 0)
    {
        printf("thread pool full\r\n");
    }

    xTaskCreate(print_delay, "print_delay", 128, NULL, 1, NULL);

	vTaskStartScheduler();

	while (1)
	{
	}
}

static void print_delay(void *arg)
{
    (void)arg;
    uint8_t i = 0;

    while(1)
    {
        vTaskDelay(10000);

        printf("execute time = ");

        for (i = 0; i < 5; i++)
        {
            printf("[%d]", delaytime[i]);
            delaytime[i] = 0;
        }
        printf("\r\n");
        periodic_task_unregister(sub_func5);
    }
}

static void sub_func1(void)
{
    static uint32_t count = 0;

    if (count != 0)
    {
        //printf("%s: running delay=%d\r\n", __FUNCTION__, get_tick() - count);
        delaytime[0] = get_tick() - count;
        led_b2_toggle();
    }
    count = get_tick();
}

static void sub_func2(void)
{
    static uint32_t count = 0;

    if (count != 0)
    {
        //printf("%s: running delay=%d\r\n", __FUNCTION__, get_tick() - count);
        delaytime[1] = get_tick() - count;
    }
    count = get_tick();
}

static void sub_func3(void)
{
    static uint32_t count = 0;

    if (count != 0)
    {
        //printf("%s: running delay=%d\r\n", __FUNCTION__, get_tick() - count);
        delaytime[2] = get_tick() - count;
    }
    count = get_tick();
}

static void sub_func4(void)
{
    static uint32_t count = 0;

    if (count != 0)
    {
        //printf("%s: running delay=%d\r\n", __FUNCTION__, get_tick() - count);
        delaytime[3] = get_tick() - count;
    }
    count = get_tick();
}

static void sub_func5(void)
{
    static uint32_t count = 0;

    if (count != 0)
    {
        //printf("%s: running delay=%d\r\n", __FUNCTION__, get_tick() - count);
        delaytime[4] = get_tick() - count;
    }
    count = get_tick();
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

