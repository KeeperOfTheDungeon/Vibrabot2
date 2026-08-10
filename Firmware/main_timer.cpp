
/**
    File: adc.h
    Authors: Oktavian Gniot & ChatGPT
    Version: 0.1 beta 2026-08-10

    Description:  
*/

#include "nrf.h"
#include "inc/main_timer.h"
#include <Arduino.h>

#define TIMER_INSTANCE   NRF_TIMER3
#define TIMER_IRQN       TIMER3_IRQn

volatile uint32_t tick_10ms = 0;  // counts 10 ms ticks

main_timer_t main_timer;

extern "C" void TIMER3_IRQHandler(void)
{
  if (NRF_TIMER3->EVENTS_COMPARE[0])
  {
        NRF_TIMER3->EVENTS_COMPARE[0] = 0;
        main_timer.tick = 1;
        main_timer.sys_time++;
  } 

}


void Main_timer::init(void)
{
    Serial.begin(115200);
    delay(5000);

    Serial.println("TEST");

     // stop timer 
    NRF_TIMER3->TASKS_STOP = 1;

    // Zimer Takt ist 16 MHz wso we use a prescaler of 4  for 1 MHz
    NRF_TIMER3->PRESCALER = 4;

    // set timer mode to Timer Mode
    NRF_TIMER3->MODE =
        TIMER_MODE_MODE_Timer;

    NRF_TIMER3->BITMODE =
        TIMER_BITMODE_BITMODE_32Bit;

    // System Takt is 10 ms so is counter value 10000 
    NRF_TIMER3->CC[0] = 10000;

    // Set compare to 0 (count Down)
    NRF_TIMER3->SHORTS =
        TIMER_SHORTS_COMPARE0_CLEAR_Msk;

    // Clear compare event
    NRF_TIMER3->EVENTS_COMPARE[0] = 0;

    // disabele Timer 3 IRQ 
    NVIC_DisableIRQ(TIMER3_IRQn);

    // Clear TIMER3_IRQ3 Pending flag
    NVIC_ClearPendingIRQ(TIMER3_IRQn);

    // enable TIMER3 Compare0 Interrupt 
    NRF_TIMER3->INTENSET =
        TIMER_INTENSET_COMPARE0_Msk;

    // The interrupt vector table is in RAM 
     
    uint32_t *vectors = (uint32_t *)SCB->VTOR;

    // set vector in interupt table for Timer3 Handler
    vectors[16 + TIMER3_IRQn] = (uint32_t)TIMER3_IRQHandler;

    __DSB();
    __ISB();

    // ISet RQ priority
    NVIC_SetPriority(TIMER3_IRQn, 3);
    
    // enable Timer3 Interrupt
    NVIC_EnableIRQ(TIMER3_IRQn);

    // Start Timer 3
    NRF_TIMER3->TASKS_START = 1;
    
}


bool Main_timer::get_tick()
{
    return(main_timer.tick);
}

void Main_timer::clear_tick()
{
    main_timer.tick = 0;
}