#include <Wire.h>
#include <arduino.h>
#include "inc/lsm6.h"


// ------------------------------------------------------------
// Write one IMU register
// ------------------------------------------------------------
bool Lsm6::writeReg(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(IMU_ADDR);
    Wire.write(reg);
    Wire.write(value);

    return Wire.endTransmission() == 0;
}

// ------------------------------------------------------------
// Read consecutive IMU registers
// ------------------------------------------------------------
bool Lsm6::readRegs(uint8_t reg, uint8_t *data, uint8_t count)
{
    Wire.beginTransmission(IMU_ADDR);
    Wire.write(reg);

    // Repeated START
    if (Wire.endTransmission(false) != 0)
        return false;

    uint8_t received = Wire.requestFrom(IMU_ADDR, count);

    if (received != count)
    {
        while (Wire.available())
            Wire.read();

        return false;
    }

    for (uint8_t i = 0; i < count; i++)
        data[i] = Wire.read();

    return true;
}

// ------------------------------------------------------------
// Convert two bytes to signed 16-bit value
// ------------------------------------------------------------
int16_t Lsm6::getS16(const uint8_t *data)
{
    return (int16_t)(
        ((uint16_t)data[1] << 8) |
        (uint16_t)data[0]
    );
}

// ------------------------------------------------------------
// Initialize onboard IMU
// ------------------------------------------------------------
bool Lsm6::imuInit()
{
    uint8_t id = 0;

    if (!readRegs(WHO_AM_I, &id, 1))
    {
        Serial.println("IMU: I2C communication error");
        return false;
    }

    if (id != 0x6A)
    {
        Serial.print("IMU: unexpected WHO_AM_I = 0x");
        Serial.println(id, HEX);
        return false;
    }

    // Block Data Update and register auto-increment
    if (!writeReg(CTRL3_C, 0x44))
        return false;

    // Accelerometer: 104 Hz, +/- 2 g
    if (!writeReg(CTRL1_XL, 0x40))
        return false;

    // Gyroscope: 104 Hz, +/- 245 dps
    if (!writeReg(CTRL2_G, 0x40))
        return false;

    delay(20);

    return true;
}

// ------------------------------------------------------------
// Setup
// ------------------------------------------------------------
void Lsm6::init()
{
    
    // Enable onboard IMU power
    NRF_P1->DIRSET = (1UL << IMU_PWR_PIN);
    NRF_P1->OUTSET = (1UL << IMU_PWR_PIN);

    delay(10);

    Wire.begin();
    Wire.setClock(I2C_CLOCK);

    Serial.println("XIAO nRF52840 Sense - LSM6DS3TR-C");

    if (!imuInit())
    {
        Serial.println("IMU initialization failed");

        while (1)
            delay(100);
    }

    Serial.println(
        "Time_ms,AccX_g,AccY_g,AccZ_g,GyroX_dps,GyroY_dps,GyroZ_dps"
    );
}