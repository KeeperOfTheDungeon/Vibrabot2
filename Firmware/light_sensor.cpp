#include <stdbool.h>
#include "inc/light_sensor.h"


void LightSensor::init()
{
 this->intensity = 0;
}


uint16_t LightSensor::getIntensity()
{
  return(this->intensity);
}
    
void LightSensor::setIntensity(uint16_t intensity)
{
  this->intensity = intensity;
}

