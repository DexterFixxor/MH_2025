/*
 * timer.c
 *
 *  Created on: Dec 5, 2025
 *      Author: lazar
 */

#include "timer.h"
#include "tim.h"

#include "module/odom/odom.h"
#include "module/bdc_motor/bdc_motor.h"

volatile uint32_t sys_ms = 0;
volatile uint32_t timeout_ms = 0;

volatile FlgTimeouts_TypeDef timeout_flags = {0};


void
set_timeout_ms(uint32_t ms)
{
	if (timeout_flags.timeout_start)
		return;

	timeout_flags.timeout_start = 1;
	timeout_flags.timeout_end = 0;
	timeout_ms = ms;
}

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

		if ((sys_ms % 2) == 0)
			odom_update();

		if ((sys_ms % 10) == 0)
			position_control_loop();

		if ((sys_ms % 2) == 0)
			motor_control_loop();


		if (timeout_flags.timeout_start)
		{
			timeout_ms--;
			if (timeout_ms == 0)
			{
				timeout_flags.timeout_end = 1;
				timeout_flags.timeout_start = 0;
			}
		}

		if ((sys_ms - (100 * 1000)) == 0) // da li je isteklo 100s
		{
			timeout_flags.timeout_100s = 1;
		}
	}



}
