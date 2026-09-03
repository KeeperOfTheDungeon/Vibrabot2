#include "nrf.h"

#include "inc/driver_motor.h"

static uint16_t pwm_values[2] = {
    255,    // P1.11 = 50 %
    255     // P1.15 = 50 %
};

void DriverMotor::init()
{
  //  NRF_P1->DIRSET = 1<<15;
   // NRF_P1->DIRSET = 1<<11;
    Serial.println("***********init Motors**********");
   // P1.11 -> PWM0 channel 0
    NRF_PWM1->PSEL.OUT[0] =
        (47 << PWM_PSEL_OUT_PIN_Pos) |
        (0 << PWM_PSEL_OUT_CONNECT_Pos);

    // P1.15 -> PWM0 channel 1
    NRF_PWM1->PSEL.OUT[1] =
        (43 << PWM_PSEL_OUT_PIN_Pos) |
        (0 << PWM_PSEL_OUT_CONNECT_Pos);

    NRF_PWM1->ENABLE = (PWM_ENABLE_ENABLE_Enabled << PWM_ENABLE_ENABLE_Pos);

    // Up counter
     NRF_PWM1->MODE = (PWM_MODE_UPDOWN_Up << PWM_MODE_UPDOWN_Pos);

    // 16 MHz PWM clock
    NRF_PWM1->PRESCALER = PWM_PRESCALER_PRESCALER_DIV_128;

    // 16 MHz / 1000 = 16 kHz
    NRF_PWM1->COUNTERTOP = 255 << PWM_COUNTERTOP_COUNTERTOP_Pos;

    NRF_PWM1->LOOP = (PWM_LOOP_CNT_Disabled << PWM_LOOP_CNT_Pos);

    // Individual duty cycle for each channel
   NRF_PWM1->DECODER = (PWM_DECODER_LOAD_Individual << PWM_DECODER_LOAD_Pos) |
                        (PWM_DECODER_MODE_RefreshCount << PWM_DECODER_MODE_Pos);

    // Sequence
    NRF_PWM1->SEQ[0].PTR = (uint32_t)pwm_values;
    NRF_PWM1->SEQ[0].CNT = 4;
    NRF_PWM1->SEQ[0].REFRESH = 0;
    NRF_PWM1->SEQ[0].ENDDELAY = 0;



    // Start sequence
    NRF_PWM1->TASKS_SEQSTART[0] = 1;
}


void DriverMotor::setMotorValues(uint8_t leftMotor, uint8_t rightMotor)
{
    pwm_values[0] = 255-leftMotor;
    pwm_values[1] = 255-rightMotor;
    Serial.println("Set Motor");
    NRF_PWM1->TASKS_SEQSTART[0] = 1;

}



