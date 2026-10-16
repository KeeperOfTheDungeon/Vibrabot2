#include "inc/TemperatureSensor.h"
#include "nrf.h"
#include "nrf_soc.h"


#include <arduino.h>
void TemperatureSensor::init(void)
{
 
}


uint16_t TemperatureSensor::read(void)
{
    int32_t temperature;

    sd_temp_get((int32_t*)&temperature);
    this->temperature = temperature;
    Serial.print("temperature :");
    Serial.println(this->temperature);
    
    return this->temperature;
}