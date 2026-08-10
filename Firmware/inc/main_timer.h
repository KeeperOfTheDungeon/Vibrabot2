#ifndef MAIN_TIMER_H_
#define MAIN_TIMER_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct{
	volatile uint32_t sys_time;
	volatile uint8_t tick:1;
	volatile uint8_t overflow:1;



}main_timer_t;

class Main_timer{
	public:
		void init(void);

		bool get_tick();
		void clear_tick();
};
#endif