#ifndef IR_SENSOR_H_
#define IR_SENSOR_H_


#include <stdint.h>
#include "inc/light_sensor.h"



class IrSensor : LightSensor{
	private:
		bool a;

	public:


		void init();
		uint16_t getIntensity();
		void setIntensity(uint16_t intensity);
};
#endif
