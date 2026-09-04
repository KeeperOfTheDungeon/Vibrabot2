#include <stdbool.h>
#include "inc/battery.h"




void Battery::init()
{
  this->capacity = 0;
}



void  Battery::setCapacity(uint16_t capacity)
{
    this->capacity = capacity;
}


uint16_t  Battery::getCapacity(void)
{
  return(this->capacity );
}