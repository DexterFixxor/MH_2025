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

#define MOTOR_VOLTAGE 		11.3 // [V]
#define MOTOR_ARR			2099
#define MOTOR_WHEEL_SEPARATION_HALF 0.079

extern volatile float
v_r_motor_measured,
v_l_motor_measured;

extern volatile float
v_r_motor_ref,
v_l_motor_ref;

extern volatile float
v_r_motor_trapez,
v_l_motor_trapez;


typedef enum
{
	FORWARD,
	BACKWARD
}MotorDir_t;

void set_motor1_dir(const MotorDir_t dir);
void set_motor2_dir(const MotorDir_t dir);

void set_motor1_voltage(float voltage);
void set_motor2_voltage(float voltage);


void set_ref_velocity(const float v, const float w);
void bdc_motor_control_loop();

#endif /* LIB_MODULE_BDC_MOTOR_BDC_MOTOR_H_ */
