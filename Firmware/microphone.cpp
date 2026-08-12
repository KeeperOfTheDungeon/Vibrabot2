#include "inc/microphone.h"



Microphone::Microphone(uint8_t clk_pin, uint8_t din_pin)
    : clk_pin(clk_pin),
      din_pin(din_pin),
      active_buffer(buffer_a),
      ready_buffer(nullptr),
      buffer_ready(false)
{
    instance = this;
}


void Microphone::begin()
{
    NRF_PDM->PSEL.CLK = clk_pin;
    NRF_PDM->PSEL.DIN = din_pin;

    NRF_PDM->MODE = PDM_MODE_OPERATION_Msk | PDM_MODE_EDGE_Msk;

    NRF_PDM->GAINL = 0x28;
    NRF_PDM->GAINR = 0x28;

    NRF_PDM->SAMPLE.PTR =
        reinterpret_cast<uint32_t>(active_buffer);

    NRF_PDM->SAMPLE.MAXCNT =
        BUFFER_SIZE;

    NRF_PDM->EVENTS_END = 0;

    NRF_PDM->INTENSET =
        PDM_INTENSET_END_Msk;

    NVIC_ClearPendingIRQ(PDM_IRQn);
    NVIC_SetPriority(PDM_IRQn, 3);
    NVIC_EnableIRQ(PDM_IRQn);

    NRF_PDM->TASKS_START = 1;
}


bool Microphone::available()
{
    return buffer_ready;
}


const int16_t* Microphone::read()
{
    if (!buffer_ready)
        return nullptr;

    buffer_ready = false;

    return 0; //ready_buffer;
}


void PDM_IRQHandler()
{
    if (!NRF_PDM->EVENTS_END)
        return;

    NRF_PDM->EVENTS_END = 0;

    // The buffer that just finished belongs to the application
    ready_buffer = active_buffer;

    // Switch to the other buffer
    if (active_buffer == buffer_a)
        active_buffer = buffer_b;
    else
        active_buffer = buffer_a;

    NRF_PDM->SAMPLE.PTR =
        reinterpret_cast<uint32_t>(active_buffer);

    NRF_PDM->SAMPLE.MAXCNT =
        BUFFER_SIZE;

    buffer_ready = true;
}

