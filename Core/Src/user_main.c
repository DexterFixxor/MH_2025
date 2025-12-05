/*
 * user_main.c
 *
 *  Created on: Dec 5, 2025
 *      Author: lazar
 */

#include "user_main.h"
#include "peripheries/timer/timer.h"
#include "tim.h"

void user_main()
{
	/* init */


	// Ukljuci brojac za TIM4
	HAL_TIM_Base_Start(&htim4);
	HAL_TIM_Base_Start_IT(&htim4);
	/* beskonacna while petlja */
	while (1)
	{
		delay_ms(1000);
		GPIOA->ODR ^= (1 << 5);
	}
}
