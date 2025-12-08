/*
 * user_main.c
 *
 *  Created on: Dec 1, 2025
 *      Author: lazar
 */

#include "user_main.h"
#include "tim.h"

/*
 * Zadatak na dan 8.12.2025.
 *
 * Kreirati FSM tako da na pritisak tastera,
 * u okviru ISR se detektuje pritisak
 * koji sluzi kao zahtev za promenu stanja
 * svako stanje je definisano drugacijom periodom
 * treperenja zelene LED
 *
 *
 */



void user_init()
{

}

void user_while()
{
	// zapocni brojanje tajmera
	 HAL_TIM_Base_Start_IT(&htim4);
	 timeout_ms_function(500);
	while (1)
	{
		// Finite State Machine


		if (flg_timeouts.flg_timeout_end)
		{

			GPIOA->ODR ^= (1 << 5);
			timeout_ms_function(500);
		}
	}
}

