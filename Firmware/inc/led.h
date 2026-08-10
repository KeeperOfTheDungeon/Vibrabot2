#ifndef LED_h_
#define LED_h_


#include <stdint.h>


#define LED_MODE_OFF	0
#define LED_MODE_ON	LED_MODE_ON + 1
#define LED_MODE_BLINK  LED_MODE_OFF + 1
#define LED_MODE_FLASH	LED_MODE_BLINK + 1

#define LED_BLINK_OFF 0
#define LED_BLINK_ON  1

typedef struct {
	uint8_t intensity;
	uint8_t phase_on;
	uint8_t phase_off;
	uint8_t phase_counter;
	uint8_t mode:3;
	uint8_t blink_state:1;
} led_t;


class Led{
	private:
		led_t led;

	public:


		void init();
		bool get_status();
		uint8_t get_mode();

		void set_intensity(uint8_t intensity);
		void process();
		void on();
		void off();
		void blink(uint8_t intensity, uint8_t phase_on, uint8_t phase_off);
		void flash(uint8_t intensity, uint8_t duration);


	void do_blink();
	void do_flash();
};

#endif
