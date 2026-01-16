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

#define MOTOR_VOLTAGE 		12.0 // [V]
#define MOTOR_ARR			2099
#define MOTOR_WHEEL_SEPARATION 0.158

#define CLIP(v, v_min, v_max) (v < v_min ? v_min : (v > v_max ? v_max : v))

typedef enum
{
	FORWARD,
	BACKWARD
}MotorDir_t;

extern volatile float
vr_m,
vl_m;

extern volatile float
vr_ref,
vl_ref;

extern volatile float
vr_trapez,
vl_trapez;

void set_motor1_dir(const MotorDir_t dir);
void set_motor2_dir(const MotorDir_t dir);

void set_motor1_voltage(float voltage);
void set_motor2_voltage(float voltage);

void motor_set_ref_vel(float v, float w);
void motor_control_loop();

#endif /* LIB_MODULE_BDC_MOTOR_BDC_MOTOR_H_ */
