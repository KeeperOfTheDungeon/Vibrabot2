#include "inc/microphone.h"


Microphone_t microphoneData;

 

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
    NRF_P0->OUTSET = (1<<10);


    NRF_PDM->MODE = PDM_MODE_OPERATION_Msk | PDM_MODE_EDGE_Msk;

    NRF_PDM->GAINL = 0x28;
    NRF_PDM->GAINR = 0x28;

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
}


bool Microphone::available()
{
    return  microphoneData.dataReady;
}


int16_t* Microphone::read()
{
    microphoneData.dataReady = 0;
    return((int16_t*)&microphoneData.data_buffer[microphoneData.readBuffer]);

  /*  if (!buffer_ready)
        return nullptr;

    buffer_ready = false;
*/
    return 0; //ready_buffer;
}


