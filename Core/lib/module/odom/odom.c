/*
 * odom.c
 *
 *  Created on: Dec 19, 2025
 *      Author: lazar
 */

#include "odom.h"
#include "peripheries/encoder/encoder.h"
#include "module/bdc_motor/bdc_motor.h"
#include <math.h>

// consts
const float C_INC2RAD = 0.00076699039; 	// [rad/inc]
const float radius_tocka = 0.075 / 2; 	// [m]
const float rastojanje_tockova = 0.255; // [m]

float dt = 0.001;
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

	v_r = enc_r * C_INC2RAD * radius_tocka / dt;
	v_l = enc_l * C_INC2RAD * radius_tocka / dt;

	v = (v_r + v_l) * 0.5;
	w = (v_r - v_l) / rastojanje_tockova;


	x += v * dt * cosf(theta + w * dt * 0.5);
	y += v * dt * sinf(theta + w * dt * 0.5);
	theta += w * dt;
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
