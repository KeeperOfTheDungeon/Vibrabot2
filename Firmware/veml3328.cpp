#include "inc/veml3328.h"
#include <Wire.h>
#include <arduino.h>

#define VEML3328_ADDR 0x10
// Registers
#define REG_CONF  0x00
#define REG_C     0x04
#define REG_R     0x05
#define REG_G     0x06
#define REG_B     0x07
#define REG_IR    0x08
#define DEV_ID    0x08

void Veml3328::init()
{
  int index;
  for (index = 0; index < VEML3328_SENSORS_COUNT; index++)
  {
     this->lightSensors[index].init();
  }
  
   Wire.begin();

  // Configuration:
  // Gain = 1x
  // Integration time = 50 ms
  // Sensor enabled
  //
  // 0x0000 is a basic/default configuration.

 // NRF_TWIM0->PSEL.SCL =
  NRF_TWIM0->PSEL.SDA = 44;

  writeRegister16(REG_CONF, 0x0000);

}


void Veml3328::writeRegister16(uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(VEML3328_ADDR);
  Wire.write(reg);
  Wire.write(value & 0xFF);
  Wire.write(value >> 8);
  Wire.endTransmission();
}

uint16_t Veml3328::readRegister16(uint8_t reg)
{
  Wire.beginTransmission(VEML3328_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(VEML3328_ADDR, (uint8_t)2);

  uint16_t value = Wire.read();
  value |= ((uint16_t)Wire.read() << 8);

  return value;
}

void Veml3328::fetchData(void)
{

  this->lightSensors[0].setIntensity(readRegister16(REG_C));
  this->lightSensors[1].setIntensity(readRegister16(REG_R));
  this->lightSensors[2].setIntensity(readRegister16(REG_G));
  this->lightSensors[3].setIntensity(readRegister16(REG_B));
  this->lightSensors[4].setIntensity(readRegister16(REG_IR));
}


uint16_t  Veml3328::getIntensity(int channel)
{
  return(this->lightSensors[channel].getIntensity());
  
}
