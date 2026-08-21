#ifndef DRIVER_MOTOR_H_
#define DRIVER_MOTOR_H_
#include <stdint.h>
#include <Arduino.h>
#include <nrf.h> 

typedef struct {
    uint8_t a;
    uint8_t b;
} motor_t;


class DriverMotor{

	public: 
	void init(void);

  
};



#endif
