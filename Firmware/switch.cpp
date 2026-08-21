#include <stdbool.h>
#include "inc/switch.h"
#include "Arduino.h"

void Switch::init(int port, int pin)
{
	  pinMode(8, OUTPUT);
}
bool Switch::getStatus(void)
{
	return(this->status);
}
void Switch::on(void)
{
	digitalWrite(8,HIGH);
	this->status = 1;

}
void Switch::off(void)
{
	digitalWrite(8,LOW);
	this->status = 0;
}
void Switch::toggle(void)
{
	if (this->status == 1)
			this->off();
		else
			this->on();
}

