#ifndef MICROPHONE_H_
#define MICROPHONE_H_

#include <Arduino.h>
#include <nrf.h>
#include <stdint.h>

#define AUDIO_BUFFER_SIZE 	256
#define AUDIO_BUFFER_COUNT 	2



typedef struct{
	int16_t data_buffer [AUDIO_BUFFER_COUNT ] [AUDIO_BUFFER_SIZE];

	volatile uint8_t writeBuffer:1;
	volatile uint8_t readBuffer:1;
	volatile uint8_t dataReady:1;
	volatile uint8_t bufferOverflow:1;
} Microphone_t; 



class Microphone
{
public:

    Microphone();
	
    void init();
    bool available();
    int16_t* read();

   bool dataReady(void);

private:
  
    uint8_t clk_pin;
    uint8_t din_pin;


};

#endif