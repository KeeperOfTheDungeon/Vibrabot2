#ifndef RGB_LED_h_
#define RGB_LED_h_
#include <stdint.h>
#include <Arduino.h>
#include <nrf.h> 

#define RGB_LED_BLUE_PIN		06
#define RGB_LED_GREEN_PIN		30
#define RGB_LED_RED_PIN			26

#define PWM_PIN_PORT 0      // Port 0


typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} rgb_led_t;



void rgb_led_init();
  




#endif
