#ifndef TEMPERATURE_SENSOR_H_
#define TEMPERATURE_SENSOR_H_


          
#include <stdint.h>


class TemperatureSensor{
	private:
	uint16_t temperature;

	public:
	void init(void);
	void fetch(void);
	uint16_t read(void);
	
};
#endif