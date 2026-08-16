#include <Arduino.h>
#include <nrf.h>   // Nordic Register-Definitionen

#include "inc/microphone.h"


Microphone microphone;

void setup() {
  Serial.begin(115200); 
    delay(1000);
  Serial.println ("init");


  microphone.init();
  Serial.println ("init complete");
}




void loop() 
{
 

    if (microphone.available())
        {
          int16_t* data = microphone.read();

        uint16_t magic = 0xAA55;
        Serial.write((uint8_t*)&magic, 2);
        Serial.write((uint8_t*)data, 256 * sizeof(int16_t));
          
        }
}

