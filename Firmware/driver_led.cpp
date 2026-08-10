#include "inc/robot_config.h"

#include "inc/driver_led.h"



static uint16_t pwm_value = 500;
void Driver_led::init()
{
  // PWM Ausschalten
  NRF_PWM0->ENABLE = 0;
/*
  NRF_PWM0->PSEL.OUT[0] =
      (LED_RGB_BLUE_PIN << PWM_PSEL_OUT_PIN_Pos)    |  // Pin-Nummer
      (PWM_PIN_PORT   << PWM_PSEL_OUT_PORT_Pos)   |  // Port (0 oder 1)
      (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);

  NRF_PWM0->PSEL.OUT[1] =
      (LED_RGB_GREEN_PIN << PWM_PSEL_OUT_PIN_Pos)    |  // Pin-Nummer
      (PWM_PIN_PORT   << PWM_PSEL_OUT_PORT_Pos)   |  // Port (0 oder 1)
      (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);

  NRF_PWM0->PSEL.OUT[2] =
      (LED_RGB_RED_PIN << PWM_PSEL_OUT_PIN_Pos)    |  // Pin-Nummer
      (PWM_PIN_PORT   << PWM_PSEL_OUT_PORT_Pos)   |  // Port (0 oder 1)
      (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);

  NRF_PWM0->PSEL.OUT[3] =
      (LED_CHARGE_PIN << PWM_PSEL_OUT_PIN_Pos)    |  // Pin-Nummer
      (PWM_PIN_PORT   << PWM_PSEL_OUT_PORT_Pos)   |  // Port (0 oder 1)
      (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);
*/
        // P0.08
    NRF_PWM0->PSEL.OUT[0] = 26 ; //LED_RGB_RED_PIN;

    NRF_PWM0->PRESCALER =  PWM_PRESCALER_PRESCALER_DIV_16;

    NRF_PWM0->COUNTERTOP = 10000;

    NRF_PWM0->SEQ[0].PTR =
        (uint32_t)&pwm_value;

    NRF_PWM0->SEQ[0].CNT = 1;

    NRF_PWM0->SEQ[0].REFRESH = 0;
    NRF_PWM0->SEQ[0].ENDDELAY = 0;

    NRF_PWM0->DECODER =
        PWM_DECODER_LOAD_Common |
        PWM_DECODER_MODE_RefreshCount;
    
    NRF_PWM0->ENABLE = 1;
    NRF_PWM0->TASKS_SEQSTART[0] = 1;
 //   digitalWrite(12,LOW);
    //digitalWrite(14,LOW);
    digitalWrite(13,HIGH); //GR
    
    Serial.println ("led driver");
}



void Driver_led::process()
{
     
}
