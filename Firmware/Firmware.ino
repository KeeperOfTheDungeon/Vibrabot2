#include <Arduino.h>
#include <nrf.h>   // Nordic Register-Definitionen

#include "inc/main_timer.h"

#include "inc/led.h"
#include "inc/light_sensor.h"
#include "inc/driver_led.h"
#include "inc/driver_adc.h"



Light_sensor left_light_sensor;
Light_sensor right_light_sensor;

Light_sensor left_ir_sensor;
Light_sensor center_ir_sensor;
Light_sensor right_ir_sensor;

Led led;
Main_timer system_clock;
Driver_led driver_led;
Driver_adc driver_adc;

void setup() {
  Serial.begin(115200); 
    delay(1000);
  Serial.println ("init");



  // Pin als Ausgang für Sicherheit (GPIO)
 // pinMode(9, OUTPUT);  // Wenn D9 wirklich P0.27 ist; sonst anpassen
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
 // pinMode(1, OUTPUT);
  // pinMode(0, OUTPUT);
 // led.init();

  left_light_sensor.init();
  right_light_sensor.init();

  left_ir_sensor.init();
  center_ir_sensor.init();
  right_ir_sensor.init();

  system_clock.init();
//  pwm_init_registers();
   driver_led.init();
  driver_adc.init();
}



volatile int  a;

#define ADC_MAX_VALUE (1<<12)-1

void process_analog_data()
{
  uint16_t value;

  value =  ADC_MAX_VALUE - driver_adc.get(0);
  left_light_sensor.set_intensity(value);


  value =  ADC_MAX_VALUE - driver_adc.get(1);
  right_light_sensor.set_intensity(value);





}


void loop() {
//  led.on();

    if (system_clock.get_tick())
    {
      system_clock.clear_tick();
      a++;
      switch (a)
      {
      case 1:
        
        //  digitalWrite(1,HIGH);
          driver_adc.adc_sample();
        break;
       
       case 10:
        
         //   digitalWrite(1,LOW);
           a=0; 
           process_analog_data();
            Serial.print ("l : ");
            Serial.print (left_light_sensor.get_intensity());
            Serial.print (" : ");
            Serial.print ("l : ");
            Serial.println (right_light_sensor.get_intensity());
            break;
      }

     
   //     pinMode(11, OUTPUT);
    //  digitalWrite(12,HIGH);
    // delay(200);
     //digitalWrite(12,LOW);
     //delay(200);

    }
}
