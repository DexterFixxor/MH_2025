/*
 * pid.c
 *
 *  Created on: 12.01.2026.
 *      Author: lazar
 */

#include "pid.h"

void PID_compute(PID_t* pid, float error)
{
	pid->output += \
			pid->Kp * (error - pid->error_k_1) + \
			pid->Ki * error + \
			pid->Kd * (error - 2 * pid->error_k_1 + pid->error_k_2);

	pid->error_k_2 = pid->error_k_1;
	pid->error_k_1 = error;

	if (pid->output > pid->out_max)
		pid->output = pid->out_max;
	else if(pid->output < pid->out_min)
		pid->output = pid->out_min;
}
