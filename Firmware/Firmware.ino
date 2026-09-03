#include <Arduino.h>
#include <nrf.h>   // Nordic Register-Definitionen

#include "inc/main_timer.h"

#include "inc/robot.h"

#include "inc/driver_led.h"




Led led;
Main_timer system_clock;
Driver_led driver_led;






Robot vibrabot;

void setup() {
  Serial.begin(115200); 
    delay(2000);
  Serial.println ("init a");




  Serial.println ("complette");
  // Pin als Ausgang für Sicherheit (GPIO)
 // pinMode(9, OUTPUT);  // Wenn D9 wirklich P0.27 ist; sonst anpassen
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  //pinMode(0, INPUT);
 // pinMode(1, OUTPUT);
  // pinMode(0, OUTPUT);
 // led.init();







  system_clock.init();


   delay(1000);
  vibrabot.init();
  
  
  Serial.println ("init complete");
  delay(1000);
}





void loop() 
{
 
  if (system_clock.get_tick())
  {
    system_clock.clear_tick();
    vibrabot.process();


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