#ifndef TEMPERATURE_SENSOR_H_
#define TEMPERATURE_SENSOR_H_


          
#include <stdint.h>


class TemperatureSensor{
	private:
	

	public:
	void init(void);
	void fetch(void);
	uint8_t read(void);
	
};
#endif