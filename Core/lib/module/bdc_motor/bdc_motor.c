/*
 * bdc_motor.c
 *
 *  Created on: Dec 26, 2025
 *      Author: lazar
 */

#include "bdc_motor.h"
#include "module/odom/odom.h"
#include <math.h>

const float
v_max = 1.0, 		// m/s
motor_acc = 1.0; 	// m/s^2 --> zeljeno ubrzanje

// PID regulator
volatile float
motor_r_output = 0.0,
motor_l_output = 0.0,
motor_r_prev_error = 0.0,
motor_l_prev_error = 0.0;

const float
Kp_r = 15.0,
Ki_r = 0.05,
Kp_l = 15.0,
Ki_l = 0.05;

volatile float
vr_m = 0,
vl_m = 0;

volatile float
vr_ref = 0.0,
vl_ref = 0.0;

volatile float
vr_trapez = 0.0,
vl_trapez = 0.0;


void motor_set_ref_vel(float v, float w)
{
	vr_ref = v + w * 0.5 * MOTOR_WHEEL_SEPARATION;
	vl_ref = v - w * 0.5 * MOTOR_WHEEL_SEPARATION;

	vr_ref = CLIP(vr_ref, -v_max, v_max);
	vl_ref = CLIP(vl_ref, -v_max, v_max);
}

void motor_control_loop()
{
	/* RIGHT */
	// trapezni profil
	if (fabsf(vr_trapez) < fabsf(vr_ref))
	{
		float acc_step = motor_acc * dt;
		float difference = vr_ref - vr_trapez;

		if (fabsf(difference) > acc_step)
		{
			if (vr_ref > 0)
				vr_trapez += acc_step;
			else if (vr_ref < 0)
				vr_trapez -= acc_step;
		}
		else
		{
			vr_trapez = vr_ref;
		}
	}
	else
	{
		vr_trapez = vr_ref;
	}




	/* LEFT */
	// trapezni profil
	if (fabsf(vl_trapez) < fabsf(vl_ref))
	{
		float acc_step = motor_acc * dt;
		float difference = vl_ref - vl_trapez;

		if (fabsf(difference) > acc_step)
		{
			if (difference > 0)
				vl_trapez += acc_step;
			else if (difference < 0)
				vl_trapez -= acc_step;
		}
		else
		{
			vl_trapez = vl_ref;
		}
	}
	else
	{
		vl_trapez = vl_ref;
	}

	// estimcija brzine pogonskog tocka [m/s]
	vr_m = v + 0.5 * w * MOTOR_WHEEL_SEPARATION;
	vl_m = v - 0.5 * w * MOTOR_WHEEL_SEPARATION;

	float error_r = vr_trapez - vr_m;
	float error_l = vl_trapez - vl_m;

	motor_r_output += Kp_r * (error_r - motor_r_prev_error) + Ki_r * error_r;
	motor_l_output += Kp_l * (error_l - motor_l_prev_error) + Ki_l * error_l;

	motor_r_prev_error = error_r;
	motor_l_prev_error = error_l;

	// saturacija izlaza iz regulatora
	motor_r_output = CLIP(motor_r_output, -MOTOR_VOLTAGE, MOTOR_VOLTAGE);
	motor_l_output = CLIP(motor_l_output, -MOTOR_VOLTAGE, MOTOR_VOLTAGE);

	set_motor1_voltage(motor_r_output);
	set_motor2_voltage(motor_l_output);

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
