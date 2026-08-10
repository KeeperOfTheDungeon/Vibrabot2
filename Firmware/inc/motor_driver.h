#ifndef MOTOR_DRIVER_h_
#define MOTOR_DRIVER_h_
#include <stdint.h>
#include <Arduino.h>
#include <nrf.h> 

typedef struct {
    uint8_t a;
    uint8_t b;
} motor_t;



void motor_driver_init();

  




#endif
