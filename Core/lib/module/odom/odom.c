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
/*
 * convert inc 2 rad, enkoder ima 2048 incrementa x4 u kvadraturnom dekodiranju
 * znaci 8192, posto pun krug ima 2 * PI radijana, jedan inkrement je
 *
 * 2 * PI / 8192
 *
 */
const float C_INC2RAD = 0.00076699039394282061485904379474597; 	// [rad/inc]
const float radius_tocka = 0.0775 / 2.0; 	// [m]
const float rastojanje_tockova = 0.265; // [m]

float dt = 0.002;
volatile float
x = 0,
y = 0,
theta = 0,
v = 0,
w = 0,
v_r = 0,
v_l = 0;


const float alpha = 0.3;

void odom_update()
{
	int32_t enc_r = enc1_get_delta_inc();
	int32_t enc_l = enc2_get_delta_inc();

	v_r = enc_r * C_INC2RAD * radius_tocka / dt;
	v_l = enc_l * C_INC2RAD * radius_tocka / dt;

	v = (v_r + v_l) / 2.0;
	w = (v_r - v_l) / rastojanje_tockova;

	// estimacija brzine pogonskih motora
	vr_m = (1 - alpha) * vr_m + alpha * (v + w * MOTOR_WHEEL_SEPARATION_HALF);
	vl_m = (1 - alpha) * vl_m + alpha * (v - w * MOTOR_WHEEL_SEPARATION_HALF);

	x += v * dt * cosf(theta + w * dt / 2.0);
	y += v * dt * sinf(theta + w * dt / 2.0);
	theta = normalize_rad_angle(theta + w * dt);

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
