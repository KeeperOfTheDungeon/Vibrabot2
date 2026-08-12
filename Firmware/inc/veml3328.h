#ifndef VEML3328_SENSOR_H_
#define VEML3328_SENSOR_H_

#include "inc/light_sensor.h"

#define VEML3328_ADDRESS    0x10

#define VEML3328_REG_CONF            0x00
#define VEML3328_REG_C               0x04
#define VEML3328_REG_R               0x05
#define VEML3328_REG_G               0x06
#define VEML3328_REG_B               0x07
#define VEML3328_REG_IR              0x08
#define VEML3328_REG_ID              0x0C

#define VEML3328_SENSORS_COUNT 		5              
#include <stdint.h>
class Veml3328{
	private:
	
	Light_sensor lightSensors[VEML3328_SENSORS_COUNT];

	public:
	void init();
	void writeRegister16(uint8_t reg, uint16_t value);
	uint16_t readRegister16(uint8_t reg);
	void fetchData(void);
	uint16_t getIntensity(int channel);
};
#endif