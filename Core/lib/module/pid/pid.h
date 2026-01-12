/*
 * pid.h
 *
 *  Created on: 12.01.2026.
 *      Author: lazar
 */

#ifndef LIB_MODULE_PID_PID_H_
#define LIB_MODULE_PID_PID_H_

typedef struct
{
	float Kp;
	float Ki;
	float Kd;
	float out_min;
	float out_max;
	volatile float output;
	volatile float error_k_1;
	volatile float error_k_2;
}PID_t;

void PID_compute(PID_t* pid, float error);

#endif /* LIB_MODULE_PID_PID_H_ */
