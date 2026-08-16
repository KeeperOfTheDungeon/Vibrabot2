

#include "inc/driver_twi.h"

/*
extern "C" void SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler()
{
    if (NRF_TWIM1->EVENTS_ERROR)
    {
        NRF_TWIM1->EVENTS_ERROR = 0;

        uint32_t error = NRF_TWIM1->ERRORSRC;
        NRF_TWIM1->ERRORSRC = error;

        //twi1.handleError();
    }

    if (NRF_TWIM1->EVENTS_STOPPED)
    {
        NRF_TWIM1->EVENTS_STOPPED = 0;

    //    twi1.handleStopped();
    }
}
*/



void Driver_twi::init()
{
/*
        
    NRF_TWIM1->ENABLE = TWIM_ENABLE_ENABLE_Enabled;

    // twi_->PSEL.SCL = scl_pin_;
    //twi_->PSEL.SDA = sda_pin_;

    NRF_TWIM1->FREQUENCY = TWIM_FREQUENCY_FREQUENCY_K400;

    NRF_TWIM1->SHORTS = 0;

    NRF_TWIM1->ERRORSRC = 0xFFFFFFFF;

    NRF_TWIM1->INTENCLR = 0xFFFFFFFF;

    NRF_TWIM1->INTENSET =
        TWIM_INTENSET_STOPPED_Msk |
        TWIM_INTENSET_ERROR_Msk;

    NVIC_DisableIRQ(PDM_IRQn);
    NVIC_ClearPendingIRQ(PDM_IRQn);


// The interrupt vector table is in RAM 
    
    uint32_t *vectors = (uint32_t *)SCB->VTOR;

    // set vector in interupt table for PDM Handler
    vectors[16 + SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn] = (uint32_t)SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler;

    __DSB();
    __ISB();

    NVIC_ClearPendingIRQ(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn);
    NVIC_SetPriority(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn, 6);
    NVIC_EnableIRQ(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn);


    NVIC_SetPriority(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn, 6);
    NVIC_EnableIRQ(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn);
*/


}