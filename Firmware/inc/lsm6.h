#ifndef LSM6_H_
#define LSM6_H_

#include <stdint.h>

// LSM6DS3TR-C register addresses
#define IMU_ADDR    0x6A

#define WHO_AM_I    0x0F
#define CTRL1_XL    0x10
#define CTRL2_G     0x11
#define CTRL3_C     0x12
#define OUTX_L_G    0x22


// XIAO nRF52840 Sense IMU power enable
#define IMU_PWR_PIN 8

#define I2C_CLOCK   400000
#define SAMPLE_MS   10

            

class Lsm6{
	private:
	
	public:
	void init();
	bool imuInit();
	int16_t getS16(const uint8_t *data);
	bool readRegs(uint8_t reg, uint8_t *data, uint8_t count);
	bool writeReg(uint8_t reg, uint8_t value);

};
#endif