#include <stdbool.h>
#include "inc/ir_sensor.h"


void IrSensor::init()
{
 LightSensor::setIntensity(0);
}


uint16_t IrSensor::getIntensity()
{
  return(LightSensor::getIntensity());
}
    
void IrSensor::setIntensity(uint16_t intensity)
{
  LightSensor::setIntensity(intensity);
}

