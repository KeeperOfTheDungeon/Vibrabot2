#ifndef LIGHT_SENSOR_H_
#define LIGHT_SENSOR_H_


#include <stdint.h>



typedef struct {
	uint16_t intensity;

} light_sensor_t;


class Light_sensor{
	private:
		light_sensor_t light_sensor;

	public:


		void init();
		uint16_t get_intensity();
		void set_intensity(uint16_t intensity);
		void set_raw_intensity(uint16_t intensity);
};

#endif
