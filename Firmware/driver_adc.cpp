#include "inc/robot_config.h"

#include <arduino.h>

#include "inc/driver_adc.h"

#define ADC_CHANNEL_COUNT 5
typedef struct 
{
    int16_t value[ADC_CHANNEL_COUNT];
} ADCValues_t;



static ADCValues_t adcBuffer[2];

// Buffer currently used by SAADC
static volatile uint8_t adcWriteBuffer = 0;

// Buffer containing the last completed sample
static volatile uint8_t adcReadBuffer = 1;

// Set when a new buffer is available
static volatile bool adcDataReady = false;




void Driver_adc::init()
{

    // Stop SAADC
    NRF_SAADC->TASKS_STOP = 1;

    // --------------------------------------------------------
    // Channel 0 -> AIN0 / P0.02
    // --------------------------------------------------------

 

    this->init_channel(0, SAADC_CH_PSELP_PSELP_AnalogInput0);
    this->init_channel(1, SAADC_CH_PSELP_PSELP_AnalogInput1);
    this->init_channel(2, SAADC_CH_PSELP_PSELP_AnalogInput2);
    this->init_channel(3, SAADC_CH_PSELP_PSELP_AnalogInput4);
    this->init_channel(4, SAADC_CH_PSELP_PSELP_AnalogInput5);

//7 battery

    // --------------------------------------------------------
    // SAADC configuration
    // --------------------------------------------------------

    NRF_SAADC->RESOLUTION =
        SAADC_RESOLUTION_VAL_12bit;

    NRF_SAADC->OVERSAMPLE =
        SAADC_OVERSAMPLE_OVERSAMPLE_Bypass;

    NRF_SAADC->RESULT.PTR =
        (uint32_t)&adcBuffer[0];

    NRF_SAADC->RESULT.MAXCNT =       ADC_CHANNEL_COUNT;


    // --------------------------------------------------------
    // Interrupt
    // --------------------------------------------------------

    NRF_SAADC->INTENSET =
        SAADC_INTENSET_END_Msk;

    NRF_SAADC->ENABLE = 1;

    // Start SAADC
    NRF_SAADC->TASKS_START = 1;

    NRF_SAADC->EVENTS_STARTED = 0;
}



void Driver_adc::adc_sample()
{
    NRF_SAADC->RESULT.PTR = (uint32_t)&adcBuffer[0]; //adcWriteBuffer];

    NRF_SAADC->RESULT.MAXCNT = ADC_CHANNEL_COUNT;

    NRF_SAADC->TASKS_START = 1;

    NRF_SAADC->TASKS_SAMPLE = 1;
}




void Driver_adc::init_channel(uint8_t adc_channel, uint8_t analog_input)
{
    NRF_SAADC->CH[adc_channel].PSELP =analog_input;
      

    NRF_SAADC->CH[adc_channel].PSELN =
        SAADC_CH_PSELN_PSELN_NC;

    NRF_SAADC->CH[adc_channel].CONFIG =
        SAADC_CH_CONFIG_RESP_Bypass |
        SAADC_CH_CONFIG_RESN_Bypass |
        SAADC_CH_CONFIG_GAIN_Gain1_6 |
        SAADC_CH_CONFIG_REFSEL_Internal |
        SAADC_CH_CONFIG_TACQ_10us |
        SAADC_CH_CONFIG_MODE_SE |
        SAADC_CH_CONFIG_BURST_Disabled;
}

uint16_t Driver_adc::get(uint8_t channel)
{
    return (adcBuffer[0].value[channel]);
}

void Driver_adc::process()
{
     
}
