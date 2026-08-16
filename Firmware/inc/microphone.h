#ifndef MICROPHONE_H_
#define MICROPHONE_H_

#include <Arduino.h>
#include <nrf.h>
#include <stdint.h>

#define AUDIO_BUFFER_SIZE 	256
#define AUDIO_BUFFER_COUNT 	2
#define FFT_SIZE 		256
#define MAGNITUDE_SIZE		FFT_SIZE/2


typedef struct{
	int16_t data_buffer [AUDIO_BUFFER_COUNT ] [AUDIO_BUFFER_SIZE];

     	float magnitude[MAGNITUDE_SIZE		];

	volatile uint8_t writeBuffer:1;
	volatile uint8_t readBuffer:1;
	volatile uint8_t dataReady:1;
	volatile uint8_t bufferOverflow:1;
	volatile uint8_t magnitudeReady:1;



} Microphone_t; 


typedef struct{
	float bin;
	float level;
} peak_t;


class Microphone
{
public:

    Microphone();
	
    void init();
    bool available();
    int16_t* read();

	bool dataReady(void);
   	void calculate_buffer(int16_t * buffer, float * fft_buffer);
	bool is_magnitudeReady(void);
	void a(void);
	
	void findMagnitudePeak(peak_t *peak);	

	void processFft(void);
private:
  
	void calculateMagnitude(float * fft_buffer);

};

#endif