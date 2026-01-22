/*
 * odom.c
 *
 *  Created on: Dec 19, 2025
 *      Author: lazar
 */

#include "odom.h"
#include "peripheries/encoder/encoder.h"
#include "module/bdc_motor/bdc_motor.h"
#include "peripheries/timer/timer.h"
#include <math.h>


const float C_INC2RAD = 0.00076699039; 	// [rad/inc]
const float radius_tocka = 0.0775 / 2; 	// [m]
const float rastojanje_tockova = 0.255; // [m]

const float beta = 1.0;

volatile float
x = 0,
y = 0,
theta = 0,
v = 0,
w = 0,
v_r = 0,
v_l = 0;



void odom_update()
{
	int32_t enc_r = enc1_get_delta_inc();
	int32_t enc_l = enc2_get_delta_inc();

	v_r = enc_r * C_INC2RAD * radius_tocka / DT;
	v_l = enc_l * C_INC2RAD * radius_tocka / DT;

	v = (v_r + v_l) * 0.5;
	w = (v_r - v_l) / rastojanje_tockova;

	v_r_motor_measured 	= (1-beta) * v_r_motor_measured +
			beta * (v + w * MOTOR_WHEEL_SEPARATION_HALF);

	v_l_motor_measured = (1-beta) * v_l_motor_measured +
			beta * (v - w * MOTOR_WHEEL_SEPARATION_HALF);

	x += v * DT * cosf(theta + w * DT / 2.0);
	y += v * DT * sinf(theta + w * DT / 2.0);
	theta += w * DT;
	theta = normalize_rad_angle(theta);

}

float normalize_rad_angle(float angle)
{
	if (angle > M_PI)
	{
		return angle - 2 * M_PI;
	}
	if (angle < -M_PI)
	{
		return angle + 2 * M_PI;
	}

	return angle;
}
