#ifndef DRIVER_ADC_H_
#define DRIVER_ADC_H_

#include <stdint.h>
#include <Arduino.h>
#include <nrf.h> 

class Driver_adc{

	public:
		void init();
		void process();
		void init_channel(uint8_t adc_channel, uint8_t analog_input);
		void adc_sample();
		uint16_t get(uint8_t channel);
};

#endif
