#ifndef ROBOT_H_
#define ROBOT_H_


#include "led.h"
#include "battery.h"
#include "ble.h"
#include "light_sensor.h"
#include "ir_sensor.h"
#include "driver_led.h"
#include "driver_adc.h"
#include "driver_motor.h"
#include "veml3328.h"
#include "microphone.h"
#include "TemperatureSensor.h"
#include "switch.h"




#define PACKAGE_VISIBLE_SENSOR_DATA 	0xA0
#define PACKAGE_IR_SENSOR_DATA 		0xA1
#define PACKAGE_FFT_DATA 		0xA2


#define PACKAGE_MOTOR_DATA	 	0xB0


typedef struct{
	uint8_t  data_type;
	uint8_t  reserved;

	uint16_t eye_left;
	uint16_t eye_right;

	uint16_t color_sensor_clear;
	uint16_t color_sensor_red;
	uint16_t color_sensor_green;
	uint16_t color_sensor_blue;
	uint16_t color_sensor_ir;

	uint16_t proximity_sensor_left[2];
	uint16_t proximity_sensor_center[2];
	uint16_t proximity_sensor_right[2];

	uint16_t battery_capacity;
	uint16_t temperature;



} Ble_sensor_data_t;


class Robot{
	private:
		uint8_t cycle_counter;
		LightSensor light_sensor_left;
 		LightSensor light_sensor_right;

		IrSensor proximity_sensor_left;
 		IrSensor proximity_sensor_center;
  		IrSensor proximity_sensor_right;
		Battery battery;

		Microphone microphone;
		TemperatureSensor temperature_sensor;


		Veml3328 color_sensor;

		Ble ble;

		Driver_adc driver_adc;
		DriverMotor driver_motor;
		Switch irSwitch;
		
		uint8_t watchdog;

		void decodeBlePackage(uint8_t * dataBlock);

		void processMotorData(uint8_t * dataBlock);
		Ble_sensor_data_t bleSensorData; 
		void prepareSensorData();
		void prepareIrSensorData();

	public:	
		void init();
		void process(void);


		void ProcessAnalogData();
		bool sendFftData();
		bool sendSensorData();

};	
#endif