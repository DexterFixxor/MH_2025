/*
 * bdc_motor.h
 *
 *  Created on: Dec 26, 2025
 *      Author: lazar
 */

#ifndef LIB_MODULE_BDC_MOTOR_BDC_MOTOR_H_
#define LIB_MODULE_BDC_MOTOR_BDC_MOTOR_H_

#include "stm32f4xx.h"
#include "main.h"


#define MOTOR_VOLTAGE 				11.19 // [V]
#define MOTOR_ARR					2100
#define MOTOR_WHEEL_SEPARATION		0.158
#define MOTOR_WHEEL_SEPARATION_HALF 0.079 // [mm]

#define CLIP(v, v_max, v_min) ((v > v_max) ? v_max : ((v < v_min) ? v_min : v))

typedef enum
{
	FORWARD,
	BACKWARD
}MotorDir_t;

extern volatile float vr_m, vl_m;

void set_motor_ref(float v, float w);
void motor_control_loop();

void set_motor1_dir(const MotorDir_t dir);
void set_motor2_dir(const MotorDir_t dir);

void set_motor1_voltage(float voltage);
void set_motor2_voltage(float voltage);

#endif /* LIB_MODULE_BDC_MOTOR_BDC_MOTOR_H_ */
