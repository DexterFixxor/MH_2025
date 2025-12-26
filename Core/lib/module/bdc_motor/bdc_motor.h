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


typedef enum
{
	FORWARD,
	BACKWARD
}MotorDir_t;

void set_motor1_dir(const MotorDir_t dir);
void set_motor2_dir(const MotorDir_t dir);

void set_motor1_voltage(float voltage);
void set_motor2_voltage(float voltage);

#endif /* LIB_MODULE_BDC_MOTOR_BDC_MOTOR_H_ */
