#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/i2c.h"
#include "libopencm3/cm3/nvic.h"
#include "libopencm3/stm32/exti.h"
#include "core/adxl345.h"

static const struct adxl345 adxl345_conf =
{
	.i2c = I2C2,
	.scl_port = GPIOB,
	.scl_pin = GPIO10,
	.sda_port = GPIOB,
	.sda_pin = GPIO11
};

static void activity_serv_init(void);
static void clock_setup(void);
static void exti_conf(void);
static void read_reg(uint8_t reg);

struct axis axis;
uint8_t source = 0;

int main(void)
{
	system_init();
	clock_setup();

	exti_conf();
	activity_serv_init();
	source = adxl345_read_byte(adxl345_conf.i2c, ADXL345_INT_SOURCE);

	while (1)
	{
		adxl345_axis_read(&adxl345_conf, &axis);
		printf("x=%d, y=%d, z=%d\r\n", axis.x, axis.y, axis.z);
		//read_int_pin();
		//source = adxl345_read_byte(adxl345_conf.i2c, ADXL345_INT_SOURCE);
		led_b2_toggle();
		delay_ms(100);		
	}

}

static void read_reg(uint8_t reg)
{
	uint8_t val = 0;
	val = adxl345_read_byte(adxl345_conf.i2c, reg);
	printf("val=%x\r\n", val);
}

static void activity_serv_init(void)
{
	adxl345_init(&adxl345_conf);
	
	adxl345_write_byte(adxl345_conf.i2c, ADXL345_POWER_CTL, 0x28);
	adxl345_range_set(&adxl345_conf, ADXL345_RANGE_16G);
	adxl345_write_byte(adxl345_conf.i2c, ADXL345_BW_RATE, ADXL345_RATE_100HZ);	

	adxl345_activity_thresh_set(&adxl345_conf, 2000);
	adxl345_activity_ctl(&adxl345_conf, ADXL345_ACT_X | ADXL345_ACT_Y | ADXL345_ACT_Z);
	adxl345_write_byte(adxl345_conf.i2c, ADXL345_ACT_INACT_CTL, 0x7f);

	adxl345_inactivity_thresh_set(&adxl345_conf, 185);
	adxl345_inactivity_ctl(&adxl345_conf, ADXL345_INACT_ACDC | ADXL345_INACT_X  \
				          | ADXL345_INACT_Y | ADXL345_INACT_Z);
	adxl345_write_byte(adxl345_conf.i2c, ADXL345_TIME_INACT, 2);	

	adxl345_freefall_thresh_set(&adxl345_conf, 750);
	adxl345_freefall_time_set(&adxl345_conf, 30);

	adxl345_interrupt_set(&adxl345_conf, INT_PIN_1, 0x1c);
}

static void clock_setup(void)
{
	rcc_periph_clock_enable(RCC_I2C2);
	rcc_periph_clock_enable(RCC_AFIO);
}

static void exti_conf(void)
{
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, 
                  GPIO_CNF_INPUT_FLOAT, GPIO1);

	/* Configure the EXTI subsystem. */
	exti_select_source(EXTI1, GPIOB);
	exti_set_trigger(EXTI1, EXTI_TRIGGER_BOTH);
	exti_enable_request(EXTI1);

    nvic_set_priority(NVIC_EXTI1_IRQ, 1);
    nvic_enable_irq(NVIC_EXTI1_IRQ);
}

void exti_isr(void)
{
    if (exti_get_flag_status(EXTI1) != EXTI1)
    {
        return;
    }
	
	source = adxl345_read_byte(adxl345_conf.i2c, ADXL345_INT_SOURCE);
	adxl345_axis_read(&adxl345_conf, &axis);
	printf("source=%x\r\n", source);

    exti_reset_request(EXTI1);
}








