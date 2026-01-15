/*
 * position.c
 *
 *  Created on: 08.01.2026.
 *      Author: lazar
 */

#include "position.h"
#include "module/bdc_motor/bdc_motor.h"
#include "module/odom/odom.h"
#include <math.h>

const float
Kp_v = 1.2,
Kp_w = 1.0;

const float
eps_dist = 0.05, // 50 mm
eps_theta = 0.01745329251994329576923690768489; // rad == 1 deg


volatile MotionState_t current_motion_state = IDLE;

void set_ref_pose(float x, float y, float theta)
{
	if (current_motion_state == IDLE)
	{
		x_ref = x;
		y_ref = y;
		theta_ref = theta;
		current_motion_state = ROTATE_TO_HEADING;
	}
}

void position_control_loop()
{
	float v_ref = 0, w_ref = 0;

	float dx = x_ref - x;
	float dy = y_ref - y;
	float heading_angle = atan2(dy, dx);

	float distance_error = sqrtf(dx * dx + dy * dy);
	float heading_error = normalize_rad_angle(heading_angle - theta);
	float theta_error = normalize_rad_angle(theta_ref - theta);

	switch(current_motion_state)
	{
	case ROTATE_TO_HEADING:
		w_ref = Kp_w * heading_error;
		v_ref = 0.0;

		if ( fabsf(heading_error) < eps_theta && fabsf(w) <= 0.01)
		{
			current_motion_state = TRANSLATE_TO_GOAL;
			w_ref = 0.0;
			v_ref = 0.0;
		}
		break;

	case TRANSLATE_TO_GOAL:
		v_ref = Kp_v * distance_error;
		if (fabsf(heading_angle) > M_PI_2)
			v_ref = -v_ref;

		w_ref = 0.1* Kp_w * heading_error;

		if (distance_error <= eps_dist && fabsf(v) < 0.05 && fabsf(w) < 0.01)
		{
			current_motion_state = ROTATE_TO_GOAL;
			v_ref = 0.0;
			w_ref = 0.0;
		}
		break;

	case ROTATE_TO_GOAL:
		w_ref = Kp_w * theta_error;
		v_ref = 0.0;

		if ( fabsf(theta_error) < eps_theta && fabsf(w) <= 0.01)
		{
			current_motion_state = GOAL_REACHED;
			w_ref = 0.0;
			v_ref = 0.0;
		}
		break;

	case GOAL_REACHED:

		break;
	}

	set_ref_velocity(v_ref, w_ref);

}
