/*
 * timer.c
 *
 *  Created on: Dec 5, 2025
 *      Author: lazar
 */

#include "timer.h"
#include "tim.h"

volatile uint32_t sys_ms = 0;

void delay_ms(uint32_t ms)
{
	uint32_t tmp = sys_ms; //copy trenutno sistemsko vreme
	while ((sys_ms - tmp) < ms);
}

void
HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM4)
	{
		sys_ms++;
	}

}
