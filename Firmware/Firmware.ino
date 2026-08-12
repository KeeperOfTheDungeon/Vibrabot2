#include <Arduino.h>
#include <nrf.h>   // Nordic Register-Definitionen

#include "inc/main_timer.h"

#include "inc/led.h"
#include "inc/ble.h"
#include "inc/light_sensor.h"
#include "inc/driver_led.h"
#include "inc/driver_adc.h"
#include "inc/veml3328.h"


Light_sensor left_light_sensor;
Light_sensor right_light_sensor;

Light_sensor left_ir_sensor;
Light_sensor center_ir_sensor;
Light_sensor right_ir_sensor;

Led led;
Main_timer system_clock;
Driver_led driver_led;
Driver_adc driver_adc;
Ble ble;
Veml3328 colorSensor;
void setup() {
  Serial.begin(115200); 
    delay(1000);
  Serial.println ("init");


  Serial.println ("complette");
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

  colorSensor.init();

  system_clock.init();
//  pwm_init_registers();
   driver_led.init();
  driver_adc.init();
   ble.init() ;
  Serial.println ("init complete");
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
#define PACKEGE_LIGHT_SENSOR_DATA 0xA0

void send_data()
{
      uint16_t value[11];
      value[0] = PACKEGE_LIGHT_SENSOR_DATA;
      value[1] = left_light_sensor.get_intensity();   // Light Sensor left
      value[2] = right_light_sensor.get_intensity();  // Light Sensor right

      value[3] = colorSensor.getIntensity(0);  // Color Sensor clear
      value[4] = colorSensor.getIntensity(1);  // Color Sensor Red
      value[5] = colorSensor.getIntensity(2);  // Color Sensor Green
      value[6] = colorSensor.getIntensity(3);  // Color Sensor Blue
      value[7] = colorSensor.getIntensity(4);  // Color Sensor IR
      Serial.print ("co : ");
      Serial.println(colorSensor.getIntensity(0));
      Serial.print ("re : ");
      Serial.println(colorSensor.getIntensity(1));
      Serial.print ("gr : ");
      Serial.println(colorSensor.getIntensity(2));
/*
      value[8] = right_light_sensor.get_intensity();  // Proximity Left
      value[9] = right_light_sensor.get_intensity();  // Proximity center
      value[10] = right_light_sensor.get_intensity();  // Proximity right

      value[10] = right_light_sensor.get_intensity();  // Acc X
      value[11] = right_light_sensor.get_intensity();  // Acc y
      value[12] = right_light_sensor.get_intensity();  // Acc z

      value[14] = right_light_sensor.get_intensity();  // Gyro X
      value[15] = right_light_sensor.get_intensity();  // Gyro y
      value[16] = right_light_sensor.get_intensity();  // Gyro z

*/

      bool succes = ble.sendDataBlock((uint8_t*)&value, 16);
    
    if (succes)
    {
      Serial.println ("succes");
    }
    else
      {
      Serial.println ("fail");
      }
    

}



void loop() {
 
    if (system_clock.get_tick())
    {
      system_clock.clear_tick();
      a++;
      switch (a)
      {
      case 1:
          Serial.println ("sample");
        //  digitalWrite(1,HIGH);
          driver_adc.adc_sample();
          colorSensor.fetchData();

          Serial.println ("sample ready");
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
            
            send_data();

            break; 

        

      }

     
   //     pinMode(11, OUTPUT);
    //  digitalWrite(12,HIGH);
    // delay(200);
     //digitalWrite(12,LOW);
     //delay(200);

    }
}



void HardFault_Handler(void)
{
    volatile uint32_t hfsr  = SCB->HFSR;
    volatile uint32_t cfsr  = SCB->CFSR;
    volatile uint32_t mmfar = SCB->MMFAR;
    
    volatile uint32_t bfar  = SCB->BFAR;

    Serial.println("HARDFAULT");

    Serial.print("HFSR: 0x");
    Serial.println(hfsr, HEX);

    Serial.print("CFSR: 0x");
    Serial.println(cfsr, HEX);

    Serial.print("MMFAR: 0x");
    Serial.println(mmfar, HEX);

    Serial.print("BFAR: 0x");
    Serial.println(bfar, HEX);

    while (1);
}