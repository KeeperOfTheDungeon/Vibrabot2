

#include "motor_driver.h"

void motor_driver_init()
{
  // PWM Ausschalten
  NRF_PWM0->ENABLE = 0;

}