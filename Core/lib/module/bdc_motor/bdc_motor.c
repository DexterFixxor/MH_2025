/*
 * bdc_motor.c
 *
 *  Created on: Dec 26, 2025
 *      Author: lazar
 */

#include "bdc_motor.h"
#include "module/pid/pid.h"
#include "module/odom/odom.h"
#include <math.h>


PID_t pid_r = {
		.Kp = 30,
		.Ki = 0.2,
		.Kd = 0,
		.out_max = MOTOR_VOLTAGE,
		.out_min = -MOTOR_VOLTAGE
};

PID_t pid_l =  {
		.Kp = 30,
		.Ki = 0.2,
		.Kd = 0,
		.out_max = MOTOR_VOLTAGE,
		.out_min = -MOTOR_VOLTAGE
};

const float v_motor_max = 1.0; // m/s
const float acc_motor = 0.4; // m/s^2

volatile float vr_m = 0, vl_m = 0;
volatile float vr_ref = 0, vl_ref = 0;
volatile float vr_trapez = 0, vl_trapez = 0;

void set_motor_ref(float v_ref, float w_ref)
{
	vr_ref = v_ref + w_ref * MOTOR_WHEEL_SEPARATION_HALF;
	vl_ref = v_ref - w_ref * MOTOR_WHEEL_SEPARATION_HALF;

	vr_ref = CLIP(vr_ref, v_motor_max, -v_motor_max);
	vl_ref = CLIP(vl_ref, v_motor_max, -v_motor_max);
}


void motor_control_loop()
{
	// Desni motor
	uint32_t ir1 = GPIOC->IDR & (0b1 << 5);
	uint32_t ir2 = GPIOC->IDR & (0b1 << 6);
	if(ir1 || ir2)
	{
		const float alpha = 0.985;
		vr_trapez = vr_trapez * alpha;
		vl_trapez = vl_trapez * alpha;
		float step = acc_motor * dt;
		if (fabs(vr_trapez) < step || fabsf(vl_trapez) < step)
		{
			vr_trapez = 0;
			vl_trapez = 0;
		}
	}
	else
	{
		float diff_r = vr_ref - vr_trapez;
		float step = acc_motor * dt;

		if (fabsf(diff_r) > step && fabsf(vr_ref) > fabsf(vr_trapez))
		{
			vr_trapez += copysignf(step, diff_r);
		}
		else
		{
			vr_trapez = vr_ref;
		}

		// Levi motor
		float diff_l = vl_ref - vl_trapez;

		if (fabsf(diff_l) > step && fabsf(vl_ref) > fabsf(vl_trapez))
		{
			vl_trapez += copysignf(step, diff_l);
		}
		else
		{
			vl_trapez = vl_ref;
		}
	}


	PID_compute(&pid_r, vr_trapez, vr_m);
	PID_compute(&pid_l, vl_trapez, vl_m);

	set_motor1_voltage(pid_r.output);
	set_motor2_voltage(pid_l.output);
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
