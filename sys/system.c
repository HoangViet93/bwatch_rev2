#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/usart.h"

#if defined(CONFIG_LED_B2)
static void led_init(void);
#endif

#if defined(CONFIG_UART_PRINTF)
static void uart_stdio_init(void);
int _write(int file, char *ptr, int len);
#endif

void system_init(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

#if defined(CONFIG_LED_B2)
	led_init();
#endif

#if defined(CONFIG_UART_PRINTF)
	uart_stdio_init();
#endif
}

#if defined(CONFIG_LED_B2)
static void led_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);

	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, 
				  GPIO_CNF_OUTPUT_PUSHPULL, GPIO2);
}
#endif

#if defined(CONFIG_UART_PRINTF)
static void uart_stdio_init(void)
{
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART1);

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
#endif

#if defined(CONFIG_FREERTOS)
void sys_tick_handler(void)
{
	osSystickHandler();
}
#endif

