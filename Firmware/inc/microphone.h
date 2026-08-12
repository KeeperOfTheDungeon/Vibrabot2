#ifndef MICROPHONE_H_
#define MICROPHONE_H_

#include <Arduino.h>
#include <nrf.h>



    int16_t buffer_a[BUFFER_SIZE];
    int16_t buffer_b[BUFFER_SIZE];

    volatile int16_t* active_buffer;
    volatile int16_t* ready_buffer;

class Microphone
{
public:
    static constexpr uint16_t BUFFER_SIZE = 128;

    Microphone(uint8_t clk_pin, uint8_t din_pin);

    void begin();
    bool available();
    const int16_t* read();

private:
    static Microphone* instance;

    uint8_t clk_pin;
    uint8_t din_pin;



    volatile bool buffer_ready;

    void handleInterrupt();

    friend void PDM0_IRQHandler();
};

#endif