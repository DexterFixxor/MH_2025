/*
 * timer.c
 *
 *  Created on: Dec 1, 2025
 *      Author: lazar
 */

#include "timer.h"
#include "stm32f4xx_hal.h"


/* extern */
Flg_Timeout_t flg_timeouts = {0};

/* Static variables */
volatile uint32_t sys_ms = 0;
volatile uint32_t timeout_ms = 0;

void
timeout_ms_function(uint32_t ms)
{
	if (flg_timeouts.flg_timeout_start)
	{
		return; // timeout brojanje je vec otpoceto
	}

	//timeout nije otpoceo, konfigurisi ga
	timeout_ms = ms;
	flg_timeouts.flg_timeout_start = 1;
	flg_timeouts.flg_timeout_end = 0;

}

void
delay_ms(uint32_t ms)
{
	const uint32_t tmp = sys_ms; // copy current time
	while ((sys_ms - tmp) < ms);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM4) // 1kHz
  {
	  sys_ms++;

	  if (sys_ms == 100 * 1000)
	  {
		  flg_timeouts.flg_100s = 1;
	  }

	  if (sys_ms == 1 * 1000) // 1s
	  {
		  flg_timeouts.flg_1s = 1;
	  }

	  if (flg_timeouts.flg_timeout_start)
	  {
		  timeout_ms--;
		  if (timeout_ms == 1)
		  {
			  flg_timeouts.flg_timeout_start = 0;
			  flg_timeouts.flg_timeout_end = 1;
		  }
	  }
  }
}
