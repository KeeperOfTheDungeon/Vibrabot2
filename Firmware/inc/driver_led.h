#ifndef DRIVER_LED_H_
#define DRIVER_LED_H_

#include <stdint.h>
#include <Arduino.h>
#include <nrf.h> 

class Driver_led{

	public:
		void init();
		void process();

};

#endif
