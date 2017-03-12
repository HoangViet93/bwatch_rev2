#include "stdint.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/cm3/systick.h"
#include "cmsis_os.h"

static void gpio_setup(void);

osThreadId thread1; 
osThreadId thread2; 

static void thread1_handle(void *arg); 
static void thread2_handle(void *arg); 

int main(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
	
	gpio_setup();

	osThreadDef(0, thread1_handle, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	osThreadDef(1, thread2_handle, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);

	thread1 = osThreadCreate(osThread(0), NULL);
	thread2 = osThreadCreate(osThread(1), NULL);

	osKernelStart();
	
	while (1);
	
	return 0;
}

static void thread1_handle(void *arg)
{
	(void)arg;
	while (1)
	{
		osDelay(100);
		gpio_toggle(GPIOB, GPIO2);
	}
}

static void thread2_handle(void *arg)
{
	(void)arg;
	while (1)
	{
		osDelay(1000);
		gpio_toggle(GPIOA, GPIO0);
	}
}


static void gpio_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);

	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO2);
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO0);
}


void sys_tick_handler(void)
{
	osSystickHandler();
}

