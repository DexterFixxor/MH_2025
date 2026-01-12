/*
 * position.c
 *
 *  Created on: 12.01.2026.
 *      Author: lazar
 */

#include "position.h"
#include "module/bdc_motor/bdc_motor.h"
#include "module/odom/odom.h"
#include "module/pid/pid.h"
#include <math.h>

const float
eps_distance = 0.01,
eps_phi = 0.00872664625997164788461845384244; // 0.5 deg

volatile MotionState_t current_motion_state = IDLE;
volatile float
x_ref = 0.0,
y_ref = 0.0,
theta_ref = 0.0;

PID_t rot_regulator = {
		.Kp = 1.0,
		.Ki = 0.1,
		.Kd = 0.1,
		.out_max = M_PI,
		.out_min = -M_PI
};

PID_t pos_regulator = {
		.Kp = 2.0,
		.Ki = 0.1,
		.Kd = 0.001,
		.out_max = 1.0,
		.out_min = -1.0
};

void set_position_ref(float x_des, float y_des, float theta_des)
{
	if (current_motion_state == IDLE)
	{
		x_ref = x_des;
		y_ref = y_des;
		theta_ref = theta_des;
		current_motion_state = ROTATE_TO_GOAL;
	}
}

void position_control_loop()
{
	float v_des = 0.0;
	float w_des = 0.0;

	float dx = x_ref - x;
	float dy = y_ref - y;
	float distance;
	float error_phi;
	float phi;
	float error_theta;

	phi = atan2f(dy, dx); //rad
	error_phi = normalize_rad_angle(phi - theta);
	distance = sqrtf(dx * dx + dy * dy);
	error_theta = normalize_rad_angle(theta_ref - theta);

	switch(current_motion_state)
	{
	case ROTATE_TO_GOAL:
//		PID_compute(&rot_regulator, error_phi);
		w_des = 3.0 * error_phi;

		if (fabsf(error_phi) < eps_phi && w == 0.0)
		{
			w_des = 0.0;
			v_des = 0.0;
			current_motion_state = TRANSLATE_TO_GOAL;
		}

		break;

	case TRANSLATE_TO_GOAL:
//		PID_compute(&pos_regulator, distance);
		if (fabsf(error_phi) > M_PI_2) // da li smo prosli cilj?
			v_des = -distance * 2.0; // ako jesmo, idi unazad
		else
			v_des = distance * 2.0;

//		PID_compute(&rot_regulator, error_phi);
		w_des = 0.2 * error_phi;

		if (distance < eps_distance && v == 0.0)
			{
				v_des = 0.0;
				w_des = 0.0;
				current_motion_state = ROTATE_TO_THETA;
			}


		break;

	case ROTATE_TO_THETA:
		w_des = 3.0 * error_theta;

		if (fabsf(error_theta) < eps_phi && w == 0.0)
		{
					w_des = 0.0;
					v_des = 0.0;
					current_motion_state = GOAL_REACHED;
		}
		break;

	case GOAL_REACHED:
		v_des = 0.0;
		w_des = 0.0;
		break;
	}

	set_motor_ref(v_des, w_des);

}
