#ifndef ADXL345_H
#define ADXL345_H

struct adxl345
{
    uint32_t i2c;
    uint32_t scl_port;
    uint16_t scl_pin;
    uint32_t sda_port;
    uint16_t sda_pin; 
};

struct axis
{
	int16_t x;
	int16_t y;
	int16_t z;
};

enum adxl345_reg
{
	ADXL345_DEVID =           0x00,
	ADXL345_THRESH_TAP =      0x1D,
	ADXL345_OFSX =            0x1E,
	ADXL345_OFSY =            0x1F,
	ADXL345_OFSZ =            0x20,
	ADXL345_DUR =             0x21,
	ADXL345_LATENT =          0x22,
	ADXL345_WINDOW =          0x23,
	ADXL345_THRESH_ACT =      0x24,
 	ADXL345_THRESH_INACT =    0x25,
	ADXL345_TIME_INACT =      0x26,
	ADXL345_ACT_INACT_CTL =   0x27,
	ADXL345_THRESH_FF =       0x28,
	ADXL345_TIME_FF =         0x29,
	ADXL345_TAP_AXES =        0x2A,
	ADXL345_ACT_TAP_STATUS =  0x2B,
	ADXL345_BW_RATE =         0x2C,
	ADXL345_POWER_CTL =       0x2D,
	ADXL345_INT_ENABLE =      0x2E,
	ADXL345_INT_MAP =         0x2F,
	ADXL345_INT_SOURCE =      0x30,
	ADXL345_DATA_FORMAT =     0x31,
	ADXL345_DATAX0 =          0x32,
	ADXL345_DATAX1 =          0x33,
	ADXL345_DATAY0 =          0x34,
	ADXL345_DATAY1 =          0x35,
	ADXL345_DATAZ0 =          0x36,
	ADXL345_DATAZ1 =          0x37,
	ADXL345_FIFO_CTL =        0x38,
	ADXL345_FIFO_STATUS =     0x39
};

enum 
{
	ADXL345_RANGE_16G          = 0x03,
  	ADXL345_RANGE_8G           = 0x02,
  	ADXL345_RANGE_4G           = 0x01,
  	ADXL345_RANGE_2G           = 0x00
};

enum 
{
	ADXL345_RATE_3200HZ =      0x0F,
	ADXL345_RATE_1600HZ =      0x0E,
	ADXL345_RATE_800HZ =       0x0D,
	ADXL345_RATE_400HZ =       0x0C,
	ADXL345_RATE_200HZ =       0x0B,
	ADXL345_RATE_100HZ =       0x0A,
	ADXL345_RATE_50HZ =        0x09,
	ADXL345_RATE_25HZ =        0x08,
	ADXL345_RATE_12HZ5 =       0x07,
	ADXL345_RATE_6HZ25=        0x06
};


#define ADXL345_DATA_READY	 (1 << 7)
#define	ADXL345_SINGLE_TAP	 (1 << 6)
#define	ADXL345_DOUBLE_TAP	 (1 << 5)
#define	ADXL345_ACTIVITY	 (1 << 4)
#define	ADXL345_INACTIVITY	 (1 << 3)
#define	ADXL345_FREE_FALL	 (1 << 2)
#define	ADXL345_WATER_MARK	 (1 << 1)
#define	ADXL345_OVERRUN		 (1 << 0)

enum 
{
	INT_PIN_1,
	INT_PIN_2
};

#define ADXL345_ACT_ACDC	(1 << 7)
#define ADXL345_ACT_X	(1 << 6)
#define ADXL345_ACT_Y	(1 << 5)
#define ADXL345_ACT_Z	(1 << 4)

#define ADXL345_INACT_ACDC	(1 << 3)
#define ADXL345_INACT_X	 (1 << 2)
#define ADXL345_INACT_Y	 (1 << 1)
#define ADXL345_INACT_Z	 (1 << 0)

int8_t adxl345_init(const struct adxl345 *conf);

void adxl345_range_set(const struct adxl345 *conf, uint8_t range) ;
void adxl345_interrupt_set(const struct adxl345 *conf, uint8_t pin, uint8_t event);
void adxl345_axis_read(const struct adxl345 *conf, struct axis *axis);

uint8_t adxl345_read_byte(uint32_t i2c, uint8_t reg);
void adxl345_write_byte(uint32_t i2c, uint8_t reg, uint8_t val);

void adxl345_activity_ctl(const struct adxl345 *conf, uint8_t value);
void adxl345_activity_thresh_set(const struct adxl345 *conf, uint16_t mg);

void adxl345_inactivity_ctl(const struct adxl345 *conf, uint8_t value);
void adxl345_inactivity_thresh_set(const struct adxl345 *conf, uint16_t mg);

void adxl345_freefall_thresh_set(const struct adxl345 *conf, uint16_t mg);
void adxl345_freefall_time_set(const struct adxl345 *conf, uint16_t ms);


#endif

