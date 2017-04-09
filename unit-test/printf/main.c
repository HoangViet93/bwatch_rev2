#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/usart.h"
#include "string.h"
#include "stdio.h"
#include "errno.h"

static void usart_setup(void);
static void clock_setup(void);
static void delay(uint32_t count);
static void led_setup(void);

int main(void)
{
	int count = 0;

	clock_setup();
	usart_setup();
	led_setup();

	while(1)
	{
		count++;
		gpio_toggle(GPIOB, GPIO2);
		printf("%d\t\n", count);
		delay(2000000);
	}
}

int _write(int file, char *ptr, int len)
{
	int i = 0;

	if (1 == file) 
	{
		for (i = 0; i < len; i++)
		{
			usart_send_blocking(USART1, ptr[i]);
		}
	}
	return len;
}

static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART1);
}

static void led_setup(void)
{
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, 
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO2);
}

static void usart_setup(void)
{
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, 
				  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO9);
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, 
				  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO10);
	
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_enable(USART1);
}

static void delay(volatile uint32_t count)
{
	while(count)
	{
		count--;
	}
}

