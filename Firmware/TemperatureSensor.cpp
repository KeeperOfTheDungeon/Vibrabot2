#include "inc/TemperatureSensor.h"
#include "nrf.h"



void TemperatureSensor::init(void)
{
 
}



void TemperatureSensor::fetch(void)
{
    NRF_TEMP->TASKS_START = 1;
}


uint8_t TemperatureSensor::read(void)
{
    int32_t temperature;

    NRF_TEMP->EVENTS_DATARDY = 0;

    temperature = NRF_TEMP->TEMP;

    NRF_TEMP->TASKS_STOP = 1;

    
    return temperature;
}