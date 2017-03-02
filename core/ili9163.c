#include "core/ili9163.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/spi.h"
#include "system.h"

static void ili9163_init_communication(const struct ili9163 *conf);
static void ili9163_init_chip(const struct ili9163 *conf);

static void ili9163_write_byte(const struct ili9163 *conf, uint8_t byte);
static void ili9163_write_reg(const struct ili9163 *conf, uint8_t reg);
static void ili9163_write_word(const struct ili9163 *conf, uint16_t word);

/*-----------------------------------------------------------------------------*/

void 
ili9163_init(const struct ili9163 *conf)
{
	ili9163_init_communication(conf);
	ili9163_init_chip(conf);
	gpio_set(conf->led_port, conf->led_pin);
}

static void 
ili9163_init_communication(const struct ili9163 *conf)
{	
	/* control pin configuration */
	gpio_set_mode(conf->cs_port, GPIO_MODE_OUTPUT_50_MHZ,		
				  GPIO_CNF_OUTPUT_PUSHPULL, conf->cs_pin);		

	gpio_set_mode(conf->rst_port, GPIO_MODE_OUTPUT_50_MHZ,		
				  GPIO_CNF_OUTPUT_PUSHPULL, conf->rst_pin);		

	gpio_set_mode(conf->a0_port, GPIO_MODE_OUTPUT_50_MHZ,		
				  GPIO_CNF_OUTPUT_PUSHPULL, conf->a0_pin);		

	gpio_set_mode(conf->led_port, GPIO_MODE_OUTPUT_50_MHZ,		
				  GPIO_CNF_OUTPUT_PUSHPULL, conf->led_pin);		

	/* spi configuration */
	if (SPI2 == conf->spi_reg)
	{
		gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,		
					  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO13);		

		gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,		
					  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO15);		
	}

	spi_init_master(conf->spi_reg, SPI_CR1_BAUDRATE_FPCLK_DIV_2, 
					SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE, 
					SPI_CR1_CPHA_CLK_TRANSITION_1,
					SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);	
	spi_enable(conf->spi_reg);
}

static void 
ili9163_init_chip(const struct ili9163 *conf)
{
	gpio_clear(conf->rst_port, conf->rst_pin);
	delay_ms(50);
	gpio_set(conf->rst_port, conf->rst_pin);
	delay_ms(50);

	ili9163_write_reg(conf, 0x1);
	delay_ms(30);
	ili9163_write_reg(conf, 0x11);
	delay_ms(120);
	
	/* powerup sequence*/
	ili9163_write_reg(conf, 0xB1);
	ili9163_write_byte(conf, 0x05);
	ili9163_write_byte(conf, 0x3C);
	ili9163_write_byte(conf, 0x3C);

	ili9163_write_reg(conf, 0xB2);
	ili9163_write_byte(conf, 0x05);
	ili9163_write_byte(conf, 0x3C);
	ili9163_write_byte(conf, 0x3C);
	
	ili9163_write_reg(conf, 0xB3);
	ili9163_write_byte(conf, 0x05);
	ili9163_write_byte(conf, 0x3C);
	ili9163_write_byte(conf, 0x3C);
	ili9163_write_byte(conf, 0x05);
	ili9163_write_byte(conf, 0x3C);
	ili9163_write_byte(conf, 0x3C);
	
	ili9163_write_reg(conf, 0xB4); 
	ili9163_write_byte(conf, 0x03);
	
	ili9163_write_reg(conf, 0xC0);
	ili9163_write_byte(conf, 0x28);
	ili9163_write_byte(conf, 0x08);
	ili9163_write_byte(conf, 0x04);
	
	ili9163_write_reg(conf, 0xC1);
	ili9163_write_byte(conf, 0XC0);
	
	ili9163_write_reg(conf, 0xC2);
	ili9163_write_byte(conf, 0x0D);
	ili9163_write_byte(conf, 0x00);
	
	ili9163_write_reg(conf, 0xC3);
	ili9163_write_byte(conf, 0x8D);
	ili9163_write_byte(conf, 0x2A);
	
	ili9163_write_reg(conf, 0xC4);
	ili9163_write_byte(conf, 0x8D);
	ili9163_write_byte(conf, 0xEE);	
	
	/* memory access mode */
	ili9163_write_reg(conf, 0xC5); 
	ili9163_write_byte(conf, 0x1A);
	
	ili9163_write_reg(conf, 0x36); 
	ili9163_write_byte(conf, 0xC8);
	
	/* gamma configuration */
	ili9163_write_reg(conf, 0xE0);
	ili9163_write_byte(conf, 0x04);
	ili9163_write_byte(conf, 0x22);
	ili9163_write_byte(conf, 0x07);
	ili9163_write_byte(conf, 0x0A);
	ili9163_write_byte(conf, 0x2E);
	ili9163_write_byte(conf, 0x30);
	ili9163_write_byte(conf, 0x25);
	ili9163_write_byte(conf, 0x2A);
	ili9163_write_byte(conf, 0x28);
	ili9163_write_byte(conf, 0x26);
	ili9163_write_byte(conf, 0x2E);
	ili9163_write_byte(conf, 0x3A);
	ili9163_write_byte(conf, 0x00);
	ili9163_write_byte(conf, 0x01);
	ili9163_write_byte(conf, 0x03);
	ili9163_write_byte(conf, 0x13);
	
	ili9163_write_reg(conf, 0xE1);
	ili9163_write_byte(conf, 0x04);
	ili9163_write_byte(conf, 0x16);
	ili9163_write_byte(conf, 0x06);
	ili9163_write_byte(conf, 0x0D);
	ili9163_write_byte(conf, 0x2D);
	ili9163_write_byte(conf, 0x26);
	ili9163_write_byte(conf, 0x23);
	ili9163_write_byte(conf, 0x27);
	ili9163_write_byte(conf, 0x27);
	ili9163_write_byte(conf, 0x25);
	ili9163_write_byte(conf, 0x2D);
	ili9163_write_byte(conf, 0x3B);
	ili9163_write_byte(conf, 0x00);
	ili9163_write_byte(conf, 0x01);
	ili9163_write_byte(conf, 0x04);
	ili9163_write_byte(conf, 0x13);
	
	/* dipslay config */
	ili9163_write_reg(conf, 0x3A);
	ili9163_write_byte(conf, 0x05);
	ili9163_write_reg(conf, 0x29); 
}



/*-----------------------------------------------------------------------------*/

void
ili9163_set_address(const struct ili9163 *conf, 
					uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2)
{
	ili9163_write_reg(conf, 0x2A);
	ili9163_write_word(conf, x1);
	ili9163_write_word(conf, x2);

	ili9163_write_reg(conf, 0x2B);
	ili9163_write_word(conf, 32 + y1);
	ili9163_write_word(conf, 32 + y2);

	ili9163_write_reg(conf, 0x2C);
}

void
ili9163_set_screen(const struct ili9163 *conf, uint16_t color)
{
	uint16_t row_index, col_index;

	ili9163_set_address(conf, 0, conf->lcd_x_size - 1, 0, conf->lcd_y_size - 1);

	for (row_index = 0; row_index < conf->lcd_y_size; row_index++)
	{
		for (col_index = 0; col_index < conf->lcd_x_size; col_index++)
		{
			ili9163_write_word(conf, color);
		}
	}
}

int8_t
ili9163_draw_pic(const struct ili9163 *conf, uint16_t x, uint16_t y, 
				 uint16_t width, uint16_t height, const uint16_t *data)
{
	uint32_t pic_index; 

	if ((x + width) >= conf->lcd_x_size || (y + height) >= conf->lcd_y_size)
	{
		return -1;
	}

	ili9163_set_address(conf, x, x + width - 1, y, y + height - 1);
	for (pic_index = 0; pic_index < width * height; pic_index++)
	{
		ili9163_write_word(conf, data[pic_index]);
	}

	return 0;
}

/*-----------------------------------------------------------------------------*/

static void
ili9163_write_byte(const struct ili9163 *conf, uint8_t byte)
{
	gpio_set(conf->a0_port, conf->a0_pin);
	
	while (!(SPI_SR(conf->spi_reg) & SPI_SR_TXE));
	spi_xfer(conf->spi_reg, byte);
}

static void
ili9163_write_reg(const struct ili9163 *conf, uint8_t reg)
{
	gpio_clear(conf->a0_port, conf->a0_pin);
	
	while (!(SPI_SR(conf->spi_reg) & SPI_SR_TXE));
	spi_xfer(conf->spi_reg, reg);
}

static void
ili9163_write_word(const struct ili9163 *conf, uint16_t word)
{
	gpio_set(conf->a0_port, conf->a0_pin);

	ili9163_write_byte(conf, word >> 8);	
	ili9163_write_byte(conf, word);
}


