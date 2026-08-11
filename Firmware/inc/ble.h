#ifndef BLE_H_
#define BLE_H_

#include <bluefruit.h>

class Ble
{
public:
    void init();

    // Robot -> PC
    bool sendDataBlock(const uint8_t* data, uint16_t length);

    // PC -> Robot
    // Returns number of bytes copied into buffer, 0 if no data available
    uint16_t receiveDataBlock(uint8_t* buffer, uint16_t maxLength);

    bool connected();

private:
    static void rxCallback(uint16_t conn_hdl,
                           BLECharacteristic* chr,
                           uint8_t* data,
                           uint16_t len);

    static volatile bool rxAvailable;

    static uint8_t rxBuffer[244];
    static volatile uint16_t rxLength;

    static BLEService service;
    static BLECharacteristic txCharacteristic;
    static BLECharacteristic rxCharacteristic;
};


#endif