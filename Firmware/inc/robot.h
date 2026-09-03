#ifndef ROBOT_H_
#define ROBOT_H_


#include "led.h"
#include "inc/ble.h"
#include "inc/light_sensor.h"
#include "inc/ir_sensor.h"
#include "inc/driver_led.h"
#include "inc/driver_adc.h"
#include "inc/driver_motor.h"
#include "inc/veml3328.h"
#include "inc/microphone.h"
#include "inc/TemperatureSensor.h"
#include "inc/switch.h"




#define PACKAGE_VISIBLE_SENSOR_DATA 	0xA0
#define PACKAGE_IR_SENSOR_DATA 		0xA1
#define PACKAGE_FFT_DATA 		0xA2


#define PACKAGE_MOTOR_DATA	 	0xB0


class Robot{
	private:
		uint8_t cycle_counter;
		LightSensor left_light_sensor;
 		LightSensor right_light_sensor;

		IrSensor left_ir_sensor;
 		IrSensor center_ir_sensor;
  		IrSensor right_ir_sensor;


		Microphone microphone;
		TemperatureSensor temperature_sensor;


		Veml3328 color_sensor;

		Ble ble;

		Driver_adc driver_adc;
		DriverMotor driver_motor;
		Switch irSwitch;
		

		void decodeBlePackage(uint8_t * dataBlock);

		void processMotorData(uint8_t * dataBlock);

	public:	
		void init();
		void process(void);


		void ProcessAnalogData();

		bool sendVisibleData();
		bool sendIrData();
		bool sendFftData();

};
#endif