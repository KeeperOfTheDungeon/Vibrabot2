#ifndef LIGHT_SENSOR_H_
#define LIGHT_SENSOR_H_


#include <stdint.h>


class LightSensor{
	private:
		uint16_t intensity;

	public:


		void init();
		uint16_t getIntensity();
		void setIntensity(uint16_t intensity);
};

#endif
