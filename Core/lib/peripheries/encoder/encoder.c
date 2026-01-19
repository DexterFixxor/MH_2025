/*
 * encoder.c
 *
 *  Created on: Dec 19, 2025
 *      Author: lazar
 */

#include "encoder.h"

void encoders_init()
{
	TIM2->CNT = 0;//ENC_START_INC;
	TIM3->CNT = 0;//ENC_START_INC;
}

int32_t enc1_get_delta_inc()
{
	uint16_t curr = TIM2->CNT;
	int32_t delta = (int16_t)(curr - prev_cnt);
	return delta;
}

int32_t enc2_get_delta_inc()
{
	uint16_t curr = TIM3->CNT;
	int32_t delta = (int16_t)(curr - prev_cnt);
	return delta;
}
