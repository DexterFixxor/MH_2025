/*
 * position.c
 *
 *  Created on: 09.01.2026.
 *      Author: lazar
 */

#include "position.h"
#include "module/odom/odom.h"
#include "module/bdc_motor/bdc_motor.h"
#include <math.h>

/* consts */
const float
Kp_rot = 1.0,
Kp_trans = 1.0;

const float
eps_theta = 0.01745329251994329576923690768489, // 1 deg
eps_distance = 0.01;

volatile MotionState_t current_motion_state = IDLE;
volatile float x_ref, y_ref, theta_ref;

void robot_set_pose_ref(float x, float y, float theta)
{
	if (current_motion_state == IDLE)
	{
		x_ref = x;
		y_ref = y;
		theta_ref = theta;

		current_motion_state = ROTATE_TO_GOAL;
	}
}

void position_control_loop()
{
	float v_ref = 0, w_ref = 0;

	float phi;
	float dx;
	float dy;

	float phi_error;
	float distance;
	float theta_error;

	dx = x_ref - x;
	dy = y_ref - y;
	phi = atan2f(dy, dx);

	phi_error = normalize_rad_angle(phi - theta);
	distance = sqrtf(dx * dx + dy * dy);
	theta_error = normalize_rad_angle(theta_ref - theta);

	switch(current_motion_state)
	{
	case ROTATE_TO_GOAL:
		v_ref = 0.0;
		w_ref = Kp_rot * phi_error;

		if (fabsf(phi_error) < eps_theta && w == 0)
		{
			v_ref = 0.0;
			w_ref = 0.0;
			current_motion_state = TRANSLATE_TO_GOAL;
		}

		break;

	case TRANSLATE_TO_GOAL:
		v_ref = Kp_trans * distance;
		w_ref = 0.2 * Kp_rot * phi_error;

		if (fabsf(phi_error) > M_PI_2)
			v_ref = -v_ref;

		if (distance < eps_distance && v == 0.0)
		{
			v_ref = 0.0;
			w_ref = 0.0;
			current_motion_state = ROTATE_TO_THETA;
		}

		break;

	case ROTATE_TO_THETA:
		v_ref = 0.0;
		w_ref = Kp_rot * theta_error;

		if (fabsf(theta_error) < eps_theta && w == 0)
		{
			v_ref = 0.0;
			w_ref = 0.0;
			current_motion_state = GOAL_REACHED;
		}


		break;

	case GOAL_REACHED:

		break;
	}

	motor_set_ref_vel(v_ref, w_ref);
}

