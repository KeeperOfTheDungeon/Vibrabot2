#ifndef SWITCH_H_
#define SWITCH_H_


#include <stdint.h>


class Switch{
	private:
		uint16_t status:1;

	public:


		void init(int port, int pin);
		bool getStatus(void);
		void on(void);
		void off(void);
		void toggle(void);
};

#endif
