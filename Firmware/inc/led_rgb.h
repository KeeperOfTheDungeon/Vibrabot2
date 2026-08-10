#ifndef LED_RGB_h_
#define LED_RGB_h_
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
} led_rgb_t;



void led_rgb_init();
void led_rgb_set_RGBColor();
void led_rgb_set_HSVColor();
void led_rgb_set_Red(uint16_t intensity);
void led_rgb_set_Green(uint16_t intensity);
void led_rgb_set_Blue(uint16_t intensity);
void led_rgb_process();


#endif
