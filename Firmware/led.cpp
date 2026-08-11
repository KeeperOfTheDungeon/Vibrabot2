#include <stdbool.h>
#include "inc/led.h"




void Led::init()
{
   led.mode = LED_MODE_OFF;	 
}


bool Led::get_status()
{
    if (led.mode == LED_MODE_OFF)
        return false;
   
   return true;
}


uint8_t Led::get_mode()
{
    return(0);
}

void Led::process()
{
    switch(led.mode)
    {
        // empty and unused
  /*      case LED_MODE_OFF:
        break;

        case LED_MODE_ON:
        break;
*/
        case LED_MODE_BLINK:
            this->do_blink();
        break;
        
        case LED_MODE_FLASH:
            this->do_flash();
        break;

    }
}



void Led::set_intensity(uint8_t intensity)
{

}

void Led::on()
{

}
void Led::off()
{

}
void Led::blink(uint8_t intensity, uint8_t phase_on, uint8_t phase_off)
{
 led.mode = LED_MODE_BLINK;
 led.intensity = intensity;
 led.phase_on = phase_on;
 led.phase_off = phase_off;
 led.blink_state = LED_BLINK_ON;
}



void Led::flash(uint8_t intensity, uint8_t duration)
{
    led.mode = LED_MODE_FLASH;
    led.intensity = intensity;
    led.phase_on = duration;
    
}





void Led::do_blink()
{
    led.phase_counter++;
}

void Led::do_flash()
{
   led. phase_counter++;
}

