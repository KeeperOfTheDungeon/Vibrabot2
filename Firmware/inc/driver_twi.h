#ifndef DRIVER_TWI_H_
#define DRIVER_TWI_H_
#include <stdint.h>
#include <Arduino.h>
#include <nrf.h> 


class Driver_twi{

	public:
		void init();
		void process();
	
};

#endif
