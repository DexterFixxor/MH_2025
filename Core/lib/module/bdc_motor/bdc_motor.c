/*
 * bdc_motor.c
 *
 *  Created on: Dec 26, 2025
 *      Author: lazar
 */

#include "bdc_motor.h"
#include "peripheries/timer/timer.h"

volatile float
v_r_motor_measured = 0,
v_l_motor_measured = 0;

volatile float
v_r_motor_ref = 0,
v_l_motor_ref = 0;

const float
v_r_max = 0.5,
v_l_max = 0.5;

const float
motor_acc = 1.0; // m/s^2

volatile float
v_r_motor_trapez = 0,
v_l_motor_trapez = 0;


// PI regulator right
volatile float
motor_output_r = 0,
Kp_r = 0.5,
Ki_r = 0.01,
prev_err_r = 0.0;

// PI regulator left
volatile float
motor_output_l = 0,
Kp_l = 0.5,
Ki_l = 0.01,
prev_err_l = 0.0;

void set_ref_velocity(const float v, const float w)
{
	v_r_motor_ref = v + w * MOTOR_WHEEL_SEPARATION_HALF;
	v_l_motor_ref = v - w * MOTOR_WHEEL_SEPARATION_HALF;

	// saturacija brzine na zadati limit
	if (v_r_motor_ref > v_r_max)
		v_r_motor_ref = v_r_max;
	else if (v_r_motor_ref < -v_r_max)
		v_r_motor_ref = -v_r_max;

	if (v_l_motor_ref > v_l_max)
		v_l_motor_ref = v_l_max;
	else if (v_l_motor_ref < -v_l_max)
		v_l_motor_ref = -v_l_max;
}

void bdc_motor_control_loop()
{
	/* DESNI */

	// trapezni profil brzine
	if (fabsf(v_r_motor_trapez) <= fabsf(v_r_motor_ref))
	{
		float step = motor_acc * DT;
		if (v_r_motor_ref > 0)
		{
			v_r_motor_trapez += step;
		}
		else if (v_r_motor_ref < 0)
		{
			v_r_motor_trapez -= step;
		}
	}
	else
	{
		v_r_motor_trapez = v_r_motor_ref;
	}

	/* LEVI */

	// trapezni profil brzine
	if (fabsf(v_l_motor_trapez) <= fabsf(v_l_motor_ref))
	{
		float step = motor_acc * DT;
		if (v_l_motor_ref > 0)
		{
			v_l_motor_trapez += step;
		}
		else if (v_l_motor_ref < 0)
		{
			v_l_motor_trapez -= step;
		}
	}
	else
	{
		v_l_motor_trapez = v_l_motor_ref;
	}

	/* PI regulacija */
	float error_r = v_r_motor_trapez - v_r_motor_measured;
	float error_l = v_l_motor_trapez - v_l_motor_measured;

	motor_output_r += Kp_r * (error_r - prev_err_r) + Ki_r * error_r;
	motor_output_l += Kp_l * (error_l - prev_err_l) + Ki_l * error_l;

	if (motor_output_r > MOTOR_VOLTAGE)
		motor_output_r = MOTOR_VOLTAGE;
	else if(motor_output_r < -MOTOR_VOLTAGE)
		motor_output_r = - MOTOR_VOLTAGE;

	if (motor_output_l > MOTOR_VOLTAGE)
		motor_output_l = MOTOR_VOLTAGE;
	else if(motor_output_l < -MOTOR_VOLTAGE)
		motor_output_l = - MOTOR_VOLTAGE;

	set_motor1_voltage(motor_output_r);
	set_motor2_voltage(motor_output_l);

}

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

