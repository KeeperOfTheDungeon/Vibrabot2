#include "inc/microphone.h"
#include <arm_math.h>

Microphone_t microphoneData;



#define SAMPLE_RATE 12500.0f



arm_rfft_fast_instance_f32 fft_instance;




extern "C" void PDM_IRQHandler()
{
   
    
    NRF_PDM->EVENTS_END = 0;

    // The buffer that just finished belongs to the application

    microphoneData.dataReady = 1;
    // Switch to the other buffer
    microphoneData.readBuffer = microphoneData.writeBuffer;
    microphoneData.writeBuffer++;
    

    NRF_PDM->SAMPLE.PTR = (uint32_t)&microphoneData.data_buffer [microphoneData.writeBuffer];
       

    NRF_PDM->SAMPLE.MAXCNT =   AUDIO_BUFFER_SIZE ;
    NRF_PDM->TASKS_START = 1;

}



Microphone::Microphone()
{
    microphoneData.writeBuffer = 0 ;
    microphoneData.dataReady = 0 ;
    microphoneData.bufferOverflow = 0 ;
}


//  _pwrPin = PIN_PDM_PWR;

#define MICROPHONE_CLK  32
#define MICROPHONE_DIN  16
#define MICROPHONE_PWR  42

void Microphone::init()
{
    NRF_P1->DIRSET = (1<<0);
    NRF_PDM->PSEL.CLK = MICROPHONE_CLK;


    NRF_P0->DIRCLR = (1<<16);
    NRF_P0->OUTCLR = (1<<16);
     NRF_PDM->PSEL.DIN = MICROPHONE_DIN;

    NRF_P1->DIRSET = (1<<10);
    NRF_P1->OUTSET = (1<<10);


    NRF_PDM->MODE = PDM_MODE_OPERATION_Msk | PDM_MODE_EDGE_Msk;


    NRF_PDM->GAINL = 0x28;
    NRF_PDM->GAINR = 0x28;
    NRF_PDM->PDMCLKCTRL = 0x08000000;
    NRF_PDM->RATIO = 0x1;

   // NRF_PDM->PRESCALER = 0x80; // prescaller
    NRF_PDM->SAMPLE.PTR = (uint32_t)&microphoneData.data_buffer [microphoneData.writeBuffer];
  

    NRF_PDM->SAMPLE.MAXCNT = AUDIO_BUFFER_SIZE ;


    NRF_PDM->EVENTS_END = 0;

    NRF_PDM->INTENSET = PDM_INTENSET_END_Msk;

  
    NVIC_DisableIRQ(PDM_IRQn);

 
    NVIC_ClearPendingIRQ(PDM_IRQn);


 // The interrupt vector table is in RAM 
     
    uint32_t *vectors = (uint32_t *)SCB->VTOR;

    // set vector in interupt table for PDM Handler
    vectors[16 + PDM_IRQn] = (uint32_t)PDM_IRQHandler;

    __DSB();
    __ISB();

    NVIC_ClearPendingIRQ(PDM_IRQn);
    NVIC_SetPriority(PDM_IRQn, 6);
    NVIC_EnableIRQ(PDM_IRQn);

    NRF_PDM->ENABLE   = PDM_ENABLE_ENABLE_Msk;

    NRF_PDM->TASKS_START = 1;
    Serial.println("Mic init completed");

    microphoneData.magnitudeReady = 0;

// init rFFT
    arm_rfft_fast_init_f32(&fft_instance, FFT_SIZE);

}


bool Microphone::available()
{
    return  microphoneData.dataReady;
}


int16_t* Microphone::read()
{
    microphoneData.dataReady = 0;

    return((int16_t*)&microphoneData.data_buffer[microphoneData.readBuffer]);
}


   
void Microphone::processFft()
{
    if (this->available())
    {
       microphoneData.dataReady = 0;

        float fft_buffer[FFT_SIZE];

        int16_t * buffer;
        buffer = &(microphoneData.data_buffer[microphoneData.readBuffer] [0]);
        this->calculate_buffer(buffer,(float*)&fft_buffer);

        this->calculateMagnitude((float*)&fft_buffer);

        microphoneData.magnitudeReady = 1;
    }
     
}



void Microphone::a(void)
{
    float max = microphoneData.magnitude[0];
    int maxIndex = 0;
    float b;

        
    

    for (int i = 1; i < MAGNITUDE_SIZE; i++)
    {
    //        Serial.println(fft_buffer[i] );
            b = microphoneData.magnitude[i];
            if (b > max)
            {
                max =b;
                maxIndex = i;
            }
        }
//  Serial.print(max);
//   Serial.print(" : ");
    Serial.println(maxIndex);

} 
    
void Microphone::calculate_buffer(int16_t * buffer, float * fft_buffer)
{
    
    for (int i = 0; i < FFT_SIZE; i++)
        {
        fft_buffer[i] = (float)microphoneData.data_buffer[0] [i];
        }
    


    arm_rfft_fast_f32(
            &fft_instance,
            fft_buffer,
            fft_buffer,
            0
        );
}



	

void Microphone::calculateMagnitude(float * fft_buffer)
{
    for (int index = 0; index <MAGNITUDE_SIZE; index++)
        {
            float real = fft_buffer[2 * index];
            float imag = fft_buffer[2 * index + 1];

           microphoneData.magnitude[index] = sqrtf(real * real + imag * imag);
        }
}

void Microphone::findMagnitudePeak(peak_t *peak)
{
    int indexMax=0;
    float max = 0;
    for (int index = 1; index < MAGNITUDE_SIZE-1; index++)
    {
        if(microphoneData.magnitude[index] > max)
        {
            max = microphoneData.magnitude[index];
            indexMax = index;
        }
    }

    float bin = 0.0f;
    bin = (indexMax-1) * microphoneData.magnitude[indexMax-1];
    bin += (indexMax) * microphoneData.magnitude[indexMax];
    bin += (indexMax+1) * microphoneData.magnitude[indexMax+1];

    float sum;
    sum = microphoneData.magnitude[indexMax-1];
    sum +=  microphoneData.magnitude[indexMax];
    sum +=  microphoneData.magnitude[indexMax+1];

    bin = bin / sum;

    microphoneData.magnitude[indexMax-1] = 0;
    microphoneData.magnitude[indexMax] = 0;
    microphoneData.magnitude[indexMax+1] = 0;
    
    peak->bin = bin;
    peak->level = sum;

}



bool Microphone::is_magnitudeReady(void)
{
    if (microphoneData.magnitudeReady == 1)
    {
        microphoneData.magnitudeReady = 0;    
        return true;
    }        

    return false;
}


/*
#define NUM_PEAKS 5

int peakBin[NUM_PEAKS];
float peakMagnitude[NUM_PEAKS];

for (int i = 0; i < NUM_PEAKS; i++)
{
    peakBin[i] = -1;
    peakMagnitude[i] = -1.0f;
}

// Suche die 5 stärksten Bins
for (int k = 1; k <= FFT_SIZE / 2; k++)
{
    float value = magnitude[k];

    for (int p = 0; p < NUM_PEAKS; p++)
    {
        if (value > peakMagnitude[p])
        {
            // Werte nach unten schieben
            for (int j = NUM_PEAKS - 1; j > p; j--)
            {
                peakMagnitude[j] = peakMagnitude[j - 1];
                peakBin[j] = peakBin[j - 1];
            }

            peakMagnitude[p] = value;
            peakBin[p] = k;

            break;
        }
    }
}
/*
// Ausgabe
for (int i = 0; i < NUM_PEAKS; i++)
{
    float frequency =
        peakBin[i] * SAMPLE_RATE / FFT_SIZE;

    Serial.print("Peak ");
    Serial.print(i);
    Serial.print(": Bin ");
    Serial.print(peakBin[i]);
    Serial.print(" = ");
    Serial.print(frequency);
    Serial.print(" Hz  Amp = ");
    Serial.println(peakMagnitude[i]);
}*/


