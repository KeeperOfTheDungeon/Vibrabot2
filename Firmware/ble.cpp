#include <bluefruit.h>

#include <bluefruit.h>
#include "inc/ble.h"


// ---------------------------------------------------------
// BLE objects
// ---------------------------------------------------------

BLEService Ble::service =
    BLEService("8b7c0001-1234-4abc-8def-123456789abc");

BLECharacteristic Ble::txCharacteristic =
    BLECharacteristic("8b7c0002-1234-4abc-8def-123456789abc");

BLECharacteristic Ble::rxCharacteristic =
    BLECharacteristic("8b7c0003-1234-4abc-8def-123456789abc");


// ---------------------------------------------------------
// RX buffer
// ---------------------------------------------------------

volatile bool Ble::rxAvailable = false;

uint8_t Ble::rxBuffer[244];

volatile uint16_t Ble::rxLength = 0;


// ---------------------------------------------------------
// Initialize BLE
// ---------------------------------------------------------

void Ble::init()
{
    Serial.begin(115200);

    Serial.println("Starting BLE...");


    Bluefruit.configPrphConn(
        247,  // MTU
        100,  // Event length
        10,   // Notification queue
        10    // Write command queue
    );

 
    Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);




    Bluefruit.begin();

    Bluefruit.setName("VibraBot");

    // -----------------------------------------------------
    // Service
    // -----------------------------------------------------

    service.begin();


    // -----------------------------------------------------
    // TX: Robot -> PC
    // Notifications
    // -----------------------------------------------------

    txCharacteristic.setProperties(
        CHR_PROPS_NOTIFY
    );

    txCharacteristic.setPermission(
        SECMODE_OPEN,
        SECMODE_NO_ACCESS
    );

    txCharacteristic.setMaxLen(244);

    txCharacteristic.begin();


    // -----------------------------------------------------
    // RX: PC -> Robot
    // Write
    // -----------------------------------------------------

    rxCharacteristic.setProperties(
        CHR_PROPS_WRITE |
        CHR_PROPS_WRITE_WO_RESP
    );

    rxCharacteristic.setPermission(
        SECMODE_OPEN,
        SECMODE_OPEN
    );

    rxCharacteristic.setMaxLen(244);

    rxCharacteristic.setWriteCallback(rxCallback);

    rxCharacteristic.begin();


    // -----------------------------------------------------
    // Advertising
    // -----------------------------------------------------

    Bluefruit.Advertising.addFlags(
        BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE
    );

    Bluefruit.Advertising.addTxPower();

    // Advertise our service
    Bluefruit.Advertising.addService(service);

    // Device name is put into scan response
    Bluefruit.ScanResponse.addName();

    // Advertising interval
    Bluefruit.Advertising.setInterval(32, 244);

    // Restart advertising after disconnect
    Bluefruit.Advertising.restartOnDisconnect(true);

    // Advertise forever
    Bluefruit.Advertising.start(0);


    Serial.println("BLE advertising started");
}


// ---------------------------------------------------------
// Robot -> PC
// ---------------------------------------------------------

bool Ble::sendDataBlock(
    const uint8_t* data,
    uint16_t length)
{
    
    if (!Bluefruit.connected())
    {
        return false;
    }
        

    if (length > 244)
        return false;
        
    digitalWrite(9 ,HIGH);    
    bool succes = txCharacteristic.notify(data, length);
    digitalWrite(9 ,LOW);    
/*
    static uint32_t packet = 0;
    static uint32_t last = 0;

    uint32_t now = micros();

    Serial.print("TX ");
    Serial.print(packet);
    Serial.print(" dt=");
    Serial.println(now - last);

    last = now;

    packet++;
*/
 //   Serial.println("TX ");
    return succes;
}


// ---------------------------------------------------------
// PC -> Robot
// ---------------------------------------------------------

void Ble::rxCallback(
    uint16_t conn_hdl,
    BLECharacteristic* chr,
    uint8_t* data,
    uint16_t len)
{
    if (len > sizeof(rxBuffer))
        len = sizeof(rxBuffer);

    memcpy(rxBuffer, data, len);

    rxLength = len;
    rxAvailable = true;
}


// ---------------------------------------------------------
// Read received data
// ---------------------------------------------------------

uint16_t Ble::receiveDataBlock(
    uint8_t* buffer,
    uint16_t maxLength)
{
    if (!rxAvailable)
        return 0;

    uint16_t length = rxLength;

    if (length > maxLength)
        length = maxLength;

    memcpy(buffer, rxBuffer, length);

    rxAvailable = false;

    return length;
}


// ---------------------------------------------------------
// Connection status
// ---------------------------------------------------------

bool Ble::connected()
{
    return Bluefruit.connected();
}

