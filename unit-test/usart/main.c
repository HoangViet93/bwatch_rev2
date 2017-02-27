#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/usart.h"
#include "string.h"
#include "stdio.h"
#include "errno.h"
#include "system.h"

static void usart_setup(void);
static void clock_setup(void);
void delay(volatile uint32_t count);
static void led_setup(void);
static void send_data(char *str);

int main(void)
{
	system_init();

	clock_setup();
	usart_setup();
	led_setup();

	while(1)
	{
		gpio_toggle(GPIOB, GPIO2);
		send_data("hello-world\r\n");
		delay(2000000);
	}
}

static void send_data(char *str)
{
	int i = 0;

	for (i = 0; i < (int)strlen(str); i++)
	{
		usart_send_blocking(USART1, str[i]);
	}
}

static void clock_setup(void)
{
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

void delay(volatile uint32_t count)
{
	while(count)
	{
		count--;
	}
}

