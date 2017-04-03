#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/i2c.h"
#include "core/adxl345.h"

#if (1)
#include "stdio.h"
#define LOG(...)	printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#define ADXL345_I2C_ADDR	(0x53)
#define ADXL345_DEVID		(0xE5)

static void _i2c_init(const struct adxl345 *conf);
static void _i2c_transmit_bytes(uint32_t i2c, const uint8_t *pbuf, uint16_t len);
static uint16_t _i2c_read_bytes(uint32_t i2c, uint8_t *pbuf, uint16_t len);

int8_t 
adxl345_init(const struct adxl345 *conf)
{
	_i2c_init(conf);
	if (ADXL345_DEVID != adxl345_read_byte(conf->i2c, ADXL345_DEVID))
	{
		return -1;
	}
	LOG("%s: init success\r\n", __FILE__);

	return 0;
}

/*----------------------------------------------------------------------------*/
void
adxl345_axis_read(const struct adxl345 *conf, struct axis *axis)
{
	uint8_t axis_buf[6];
	uint8_t reg = ADXL345_DATAX0;

	_i2c_transmit_bytes(conf->i2c, &reg, 1);
	_i2c_read_bytes(conf->i2c, axis_buf, 6);
	
	axis->x = (((int)axis_buf[1]) << 8) + axis_buf[0];
	axis->y = (((int)axis_buf[3]) << 8) + axis_buf[2];
	axis->z = (((int)axis_buf[5]) << 8) + axis_buf[4];	
}

void 
adxl345_range_set(const struct adxl345 *conf, uint8_t range) 
{
	uint8_t data = adxl345_read_byte(conf->i2c, ADXL345_DATA_FORMAT);
	data &= 0x00; 
	data |= range; 
	data |= 0x08; 
	adxl345_write_byte(conf->i2c, ADXL345_DATA_FORMAT, data);
}

void 
adxl345_activity_thresh_set(const struct adxl345 *conf, uint16_t mg) 
{
	adxl345_write_byte(conf->i2c, ADXL345_THRESH_ACT, (uint8_t)(mg / 62.5));
}

void 
adxl345_activity_ctl(const struct adxl345 *conf, uint8_t value)
{
	uint8_t reg = 0;
	reg = adxl345_read_byte(conf->i2c, ADXL345_ACT_INACT_CTL);
	reg &= ~0xF0;
	reg |= value;
	adxl345_write_byte(conf->i2c, ADXL345_ACT_INACT_CTL ,reg);
}

void 
adxl345_inactivity_thresh_set(const struct adxl345 *conf, uint16_t mg) 
{
	adxl345_write_byte(conf->i2c, ADXL345_THRESH_INACT, (uint8_t)(mg / 62.5));
}

void 
adxl345_inactivity_ctl(const struct adxl345 *conf, uint8_t value)
{
	uint8_t reg = 0;
	reg = adxl345_read_byte(conf->i2c, ADXL345_ACT_INACT_CTL);
	reg &= ~0x0F;
	reg |= value;
	adxl345_write_byte(conf->i2c, ADXL345_ACT_INACT_CTL ,reg);
}

void adxl345_interrupt_set(const struct adxl345 *conf, uint8_t pin, uint8_t event)
{
	if (INT_PIN_1 == pin)
	{
		adxl345_write_byte(conf->i2c, ADXL345_INT_MAP, 0x00);
	}
	else
	{
		adxl345_write_byte(conf->i2c, ADXL345_INT_MAP, 0xFF);
	}
	adxl345_write_byte(conf->i2c, ADXL345_INT_ENABLE, event);
}

void
adxl345_freefall_thresh_set(const struct adxl345 *conf, uint16_t mg) 
{
	adxl345_write_byte(conf->i2c, ADXL345_THRESH_FF, (uint8_t)(mg / 62.5));
}

void
adxl345_freefall_time_set(const struct adxl345 *conf, uint16_t ms) 
{
	adxl345_write_byte(conf->i2c, ADXL345_TIME_FF, (uint8_t)(ms / 5));
}
/*----------------------------------------------------------------------------*/
uint8_t
adxl345_read_byte(uint32_t i2c, uint8_t reg)
{
	uint8_t data = 0;
	_i2c_transmit_bytes(i2c, &reg, 1);
	_i2c_read_bytes(i2c, &data, 1);
	return data;
}

void 
adxl345_write_byte(uint32_t i2c, uint8_t reg, uint8_t val)
{
	uint8_t data[2] = {reg, val};
	_i2c_transmit_bytes(i2c, data, 2);
}
/*----------------------------------------------------------------------------*/
static void
_i2c_init(const struct adxl345 *conf)
{
	gpio_set_mode(conf->scl_port, GPIO_MODE_OUTPUT_50_MHZ,
				GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		 		conf->scl_pin);
	gpio_set_mode(conf->sda_port, GPIO_MODE_OUTPUT_50_MHZ,
				GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		 		conf->sda_pin);

	i2c_peripheral_disable(conf->i2c);
	i2c_set_clock_frequency(conf->i2c, I2C_CR2_FREQ_36MHZ);
	i2c_set_fast_mode(conf->i2c);
	i2c_set_ccr(conf->i2c, 0x1e);
	i2c_set_trise(conf->i2c, 0x0b);
	i2c_set_own_7bit_slave_address(conf->i2c, 0x00);
	i2c_peripheral_enable(conf->i2c);
}

static void
_i2c_transmit_bytes(uint32_t i2c, const uint8_t *pbuf, uint16_t len)
{
	uint32_t reg32 __attribute__((unused));
	uint16_t i = 0;

	i2c_send_start(i2c);

	while (!((I2C_SR1(i2c) & I2C_SR1_SB)
		& (I2C_SR2(i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

	i2c_send_7bit_address(i2c, ADXL345_I2C_ADDR, I2C_WRITE);

	while (!(I2C_SR1(i2c) & I2C_SR1_ADDR));
	reg32 = I2C_SR2(i2c);

	for (i = 0; i < len; i++)
	{
		i2c_send_data(i2c, pbuf[i]); 
		while (!(I2C_SR1(i2c) & (I2C_SR1_BTF | I2C_SR1_TxE)));
	}

	I2C_CR1(i2c) |= I2C_CR1_STOP;
	I2C_CR1(i2c) &= ~I2C_CR1_POS;
}

static uint16_t 
_i2c_read_bytes(uint32_t i2c, uint8_t *pbuf, uint16_t len)
{
	uint32_t reg32 __attribute__((unused));
	uint16_t i = 0;

	i2c_send_start(i2c);

	while (!((I2C_SR1(i2c) & I2C_SR1_SB)
		& (I2C_SR2(i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

	i2c_send_7bit_address(i2c, ADXL345_I2C_ADDR, I2C_READ);

	while (!(I2C_SR1(i2c) & I2C_SR1_ADDR));
	reg32 = I2C_SR2(i2c);

	switch (len)
	{
	case 1:
		break;
	case 2:
		I2C_CR1(i2c) |= (I2C_CR1_POS | I2C_CR1_ACK);
		while (!(I2C_SR1(i2c) & I2C_SR1_BTF));
		break;
	default:
		I2C_CR1(i2c) |= (I2C_CR1_ACK);
		while (i < (len - 3)) 
		{
			while (!(I2C_SR1(i2c) & I2C_SR1_RxNE));
			pbuf[i++] = I2C_DR(i2c); 
		}
		while (!(I2C_SR1(i2c) & I2C_SR1_BTF));
		pbuf[i++] = I2C_DR(i2c); 
	}

	I2C_CR1(i2c) &= ~(I2C_CR1_ACK);
	I2C_CR1(i2c) |= I2C_CR1_STOP;
	
	while (i < len)
	{
		while (!(I2C_SR1(i2c) & I2C_SR1_RxNE));
		pbuf[i++] = I2C_DR(i2c);
	}
	while(I2C_CR1(i2c) & I2C_CR1_STOP);

	I2C_CR1(i2c) &= ~I2C_CR1_POS;
	I2C_CR1(i2c) |= (I2C_CR1_ACK);

	return len;
}

