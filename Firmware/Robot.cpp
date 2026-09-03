#include <bluefruit.h>
#include "inc/robot.h"


#include "inc/led.h"
#include "inc/ble.h"
#include "inc/light_sensor.h"
#include "inc/ir_sensor.h"
#include "inc/driver_led.h"
#include "inc/driver_adc.h"
#include "inc/driver_motor.h"
#include "inc/veml3328.h"
#include "inc/microphone.h"
#include "inc/TemperatureSensor.h"


void Robot::init()
{
   Serial.println("***********init **********");
  ble.init();

 // microphone.init();

  this->left_light_sensor.init();
  this->right_light_sensor.init();


  this->left_ir_sensor.init();
  this->center_ir_sensor.init();
  this->right_ir_sensor.init();
  this->color_sensor.init();
  this->irSwitch.init(0,0);

 Serial.println("***********init adc**********");

  driver_adc.init();

  this->driver_motor.init();
}


#define ADC_MAX_VALUE (1<<12)-1

void Robot::ProcessAnalogData()
{
  uint16_t value;
  value =  ADC_MAX_VALUE - driver_adc.get(0);
  this->left_light_sensor.setIntensity(value);

  value =  ADC_MAX_VALUE - driver_adc.get(1);
  this->right_light_sensor.setIntensity(value);


  value =  ADC_MAX_VALUE - driver_adc.get(2);
  this->left_ir_sensor.setIntensity(value);   
  
  value =  ADC_MAX_VALUE - driver_adc.get(3);
  this->center_ir_sensor.setIntensity(value);
  
  value =  ADC_MAX_VALUE - driver_adc.get(4);
  this->right_ir_sensor.setIntensity(value);
}




bool Robot::sendVisibleData()
{
      uint16_t value[11];
      value[0] = PACKAGE_VISIBLE_SENSOR_DATA ;
      value[1] = left_light_sensor.getIntensity();   // Light Sensor left
      value[2] = right_light_sensor.getIntensity();  // Light Sensor right


      value[3] = color_sensor.getIntensity(0);  // Color Sensor clear
      value[4] = color_sensor.getIntensity(1);  // Color Sensor Red
      value[5] = color_sensor.getIntensity(2);  // Color Sensor Green
      value[6] = color_sensor.getIntensity(3);  // Color Sensor Blue
      value[7] = color_sensor.getIntensity(4);  // Color Sensor IR

      bool succes = ble.sendDataBlock((uint8_t*)value, 16);
    
  return succes;
}



bool Robot::sendIrData()
{
      uint16_t value[5];
      value[0] = PACKAGE_IR_SENSOR_DATA ;
      value[1] = this->left_ir_sensor.getIntensity();   // ir Sensor left
      value[2] = this->center_ir_sensor.getIntensity();  // ir Sensor right
      value[3] = this->right_ir_sensor.getIntensity();  // ir Sensor clear
      value[4] = this->irSwitch.getStatus(); // status of the ir leds

      bool success = ble.sendDataBlock((uint8_t*)value, 30);
   

  return success;
}


bool Robot::sendFftData()
{
  peak_t peak;
  int16_t bin;
  int16_t level;
  uint16_t value[7];
  uint8_t pos;

  value[0] = PACKAGE_FFT_DATA;

  pos = 1;
  for (int i =0; i < 3 ;i++)
  {
    microphone.findMagnitudePeak(&peak);
   
    bin = (uint16_t) (peak.bin*100);
    value[pos++] = bin;
    level = (uint16_t)  (peak.level/230);
  
    value[pos++] = level;
  }
   
    
   bool succes = ble.sendDataBlock((uint8_t*)value, 14);

    return succes;
}


void Robot::process(void)
{
 
//Serial.println("alive");

 
  if (this->microphone.is_magnitudeReady())
  {
    this->sendFftData();

  }
  this->microphone.processFft();

  this->cycle_counter++;
  switch (this->cycle_counter)
    {
    case 1:
        this->irSwitch.on();
        this->driver_adc.adc_sample();

         this->color_sensor.fetchData();
      break;
      
      case 2:
          this->ProcessAnalogData();
          this->sendVisibleData();
      break;


      case 4:
          this->sendIrData();
      break;

    case 5:
        this->irSwitch.off();
        this->driver_adc.adc_sample();
      break;
      
      case 7:
          this->ProcessAnalogData();
          this->sendIrData();
      break;


      case 10:
      
          this->cycle_counter=0; 
      break; 
     

    }


    uint8_t  dataBlock[20];
    uint16_t status = this->ble.receiveDataBlock(dataBlock,20);
    if (status>0)
    {
      //  Serial.println(status);
      //  uint32_t now = micros();
        //Serial.println(now);
        Serial.printBuffer(dataBlock,3);
        Serial.println("");
    }

}
