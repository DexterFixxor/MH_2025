/*
 * encoder.c
 *
 *  Created on: Dec 19, 2025
 *      Author: lazar
 */

#include "encoder.h"

void encoders_init()
{
	TIM2->CNT = ENC_START_INC;
	TIM3->CNT = ENC_START_INC;
}

int32_t enc1_get_delta_inc()
{
	int32_t tmp = (TIM2->CNT - ENC_START_INC);
	TIM2->CNT = ENC_START_INC;
	return tmp;
}

int32_t enc2_get_delta_inc()
{
	int32_t tmp = (TIM3->CNT - ENC_START_INC);
	TIM3->CNT = ENC_START_INC;
	return tmp;
}
