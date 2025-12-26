/*
 * bdc_motor.c
 *
 *  Created on: Dec 26, 2025
 *      Author: lazar
 */

#include "bdc_motor.h"

void set_motor1_dir(const MotorDir_t dir)
{
	switch(dir)
	{
	case FORWARD:
		HAL_GPIO_WritePin(M1_DIR_GPIO_Port, M1_DIR_Pin, 0);
		break;

	case BACKWARD:
		HAL_GPIO_WritePin(M1_DIR_GPIO_Port, M1_DIR_Pin, 1);
		break;
	}
}

void set_motor2_dir(const MotorDir_t dir)
{
	switch(dir)
	{
	case FORWARD:
		HAL_GPIO_WritePin(M2_DIR_GPIO_Port, M2_DIR_Pin, 0);
		break;

	case BACKWARD:
		HAL_GPIO_WritePin(M2_DIR_GPIO_Port, M2_DIR_Pin, 1);
		break;
	}
}

void set_motor1_voltage(float voltage)
{
	// saturacija napona
	if (voltage > MOTOR_VOLTAGE)
		voltage = MOTOR_VOLTAGE;
	else if (voltage < -MOTOR_VOLTAGE)
		voltage = -MOTOR_VOLTAGE;


	if (voltage > 0)
	{
		set_motor1_dir(FORWARD);
		TIM1->CCR1 = (uint32_t)(voltage * MOTOR_ARR/MOTOR_VOLTAGE);
	}
	else if (voltage < 0)
	{
		set_motor1_dir(BACKWARD);
		TIM1->CCR1 = (uint32_t)(-voltage * MOTOR_ARR/MOTOR_VOLTAGE);
	}

}

void set_motor2_voltage(float voltage)
{
	// saturacija napona
	if (voltage > MOTOR_VOLTAGE)
		voltage = MOTOR_VOLTAGE;
	else if (voltage < -MOTOR_VOLTAGE)
		voltage = -MOTOR_VOLTAGE;


	if (voltage > 0)
	{
		set_motor2_dir(FORWARD);
		TIM1->CCR2 = (uint32_t)(voltage * MOTOR_ARR/MOTOR_VOLTAGE);
	}
	else if (voltage < 0)
	{
		set_motor2_dir(BACKWARD);
		TIM1->CCR2 = (uint32_t)(-voltage * MOTOR_ARR/MOTOR_VOLTAGE);
	}
}
