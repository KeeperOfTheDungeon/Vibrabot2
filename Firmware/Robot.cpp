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
  this->battery.init();
  this->light_sensor_left.init();
  this->light_sensor_right.init();


  this->proximity_sensor_left.init();
  this->proximity_sensor_center.init();
  this->proximity_sensor_right.init();
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
  this->light_sensor_left.setIntensity(value);

  value =  ADC_MAX_VALUE - driver_adc.get(1);
  this->light_sensor_right.setIntensity(value);


  value =  ADC_MAX_VALUE - driver_adc.get(2);
  this->proximity_sensor_left.setIntensity(value);   
  
  value =  ADC_MAX_VALUE - driver_adc.get(3);
  this->proximity_sensor_center.setIntensity(value);
  
  value =  ADC_MAX_VALUE - driver_adc.get(4);
  this->proximity_sensor_right.setIntensity(value);


  value = driver_adc.get(5);
  this->battery.setCapacity(value);
  Serial.print("bat :");
  Serial.println(value);

}

void Robot::prepareSensorData()
{

  this->bleSensorData.data_type = PACKAGE_VISIBLE_SENSOR_DATA;

  this->bleSensorData.eye_left = this->light_sensor_left.getIntensity();
  this->bleSensorData.eye_right = this->light_sensor_right.getIntensity();

  this->bleSensorData.color_sensor_clear = this->color_sensor.getIntensity(0);
  this->bleSensorData.color_sensor_red = this->color_sensor.getIntensity(1);
  this->bleSensorData.color_sensor_green = this->color_sensor.getIntensity(2);
  this->bleSensorData.color_sensor_blue = this->color_sensor.getIntensity(3);
  this->bleSensorData.color_sensor_ir = this->color_sensor.getIntensity(4);

  this->bleSensorData.proximity_sensor_left[0] =  this->proximity_sensor_left.getIntensity(); 
  this->bleSensorData.proximity_sensor_center[0] = this->proximity_sensor_center.getIntensity(); 
  this->bleSensorData.proximity_sensor_right[0] =  this->proximity_sensor_right.getIntensity(); 

   this->bleSensorData.battery_capacity = this->battery.getCapacity();
   this->bleSensorData.temperature = 6;
}


void Robot::prepareIrSensorData()
{
  this->bleSensorData.proximity_sensor_left[1] =  this->proximity_sensor_left.getIntensity(); 
  this->bleSensorData.proximity_sensor_center[1] = this->proximity_sensor_center.getIntensity(); 
  this->bleSensorData.proximity_sensor_right[1] =  this->proximity_sensor_right.getIntensity(); 

}

bool Robot::sendSensorData()
{
      bool succes = ble.sendDataBlock((uint8_t*)&this->bleSensorData, sizeof(Ble_sensor_data_t));
      //if (not succes)   red led
  return succes;
}




void Robot::decodeBlePackage(uint8_t * dataBlock)
{

  uint8_t command =  dataBlock[0];


  Serial.print("Decode");
  Serial.println(command);

  switch(command)
  {
    case PACKAGE_MOTOR_DATA	 : 
      this->processMotorData(dataBlock);
      break;
  }

}



void Robot::processMotorData(uint8_t * dataBlock)
{
  uint8_t leftMotor = dataBlock[1];
  uint8_t rightMotor = dataBlock[2];

   this->driver_motor.setMotorValues(leftMotor, rightMotor);
     Serial.print("Set Motors");
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
          this->prepareSensorData();
      break;

    case 5:
        this->irSwitch.off();
        this->driver_adc.adc_sample();
      break;
      
      case 7:
          this->ProcessAnalogData();
          this->prepareIrSensorData();
      break;

      case 8:
          this->sendSensorData();
      break;


      case 10:
      
          this->cycle_counter=0; 
      break; 
     

    }


    uint8_t  dataBlock[20];
    uint16_t status = this->ble.receiveDataBlock(dataBlock,20);
    if (status>0)
    {
      this->decodeBlePackage(dataBlock);
      //  Serial.println(status);
      //  uint32_t now = micros();
        //Serial.println(now);
        Serial.printBuffer(dataBlock,3);
        Serial.println("");
    }

}
