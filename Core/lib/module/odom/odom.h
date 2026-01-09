/*
 * odom.h
 *
 *  Created on: Dec 19, 2025
 *      Author: lazar
 */

#ifndef LIB_MODULE_ODOM_ODOM_H_
#define LIB_MODULE_ODOM_ODOM_H_

#include "stm32f4xx.h"

extern volatile float
x,
y,
theta,
v,
w,
v_r,
v_l;

extern float dt;

void odom_update();

/*
 * Normalize angle in range [-PI, PI]
 */
float normalize_rad_angle(float angle);

#endif /* LIB_MODULE_ODOM_ODOM_H_ */
