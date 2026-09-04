#ifndef BATTERY_H_
#define BATTERY_H_

#include <stdint.h>

class Battery
{
public:
	void init();
	uint16_t  getCapacity(void);
	void  setCapacity(uint16_t capacity);
private:
	uint16_t capacity;
};


#endif