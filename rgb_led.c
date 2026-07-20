

#include "rgb_led.h"

void rgb_led_init()
{
  // PWM Ausschalten
  NRF_PWM0->ENABLE = 0;

  NRF_PWM0->PSEL.OUT[0] =
      (RGB_LED_BLUE_PIN << PWM_PSEL_OUT_PIN_Pos)    |  // Pin-Nummer
      (PWM_PIN_PORT   << PWM_PSEL_OUT_PORT_Pos)   |  // Port (0 oder 1)
      (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);

  NRF_PWM0->PSEL.OUT[1] =
      (RGB_LED_GREEN_PIN << PWM_PSEL_OUT_PIN_Pos)    |  // Pin-Nummer
      (PWM_PIN_PORT   << PWM_PSEL_OUT_PORT_Pos)   |  // Port (0 oder 1)
      (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);

  NRF_PWM0->PSEL.OUT[2] =
      (RGB_LED_RED_PIN << PWM_PSEL_OUT_PIN_Pos)    |  // Pin-Nummer
      (PWM_PIN_PORT   << PWM_PSEL_OUT_PORT_Pos)   |  // Port (0 oder 1)
      (PWM_PSEL_OUT_CONNECT_Connected << PWM_PSEL_OUT_CONNECT_Pos);


  // 3. PWM-Modus einstellen
  NRF_PWM0->MODE = PWM_MODE_UPDOWN_Up;   // Einfacher Up-Zählmodus

}