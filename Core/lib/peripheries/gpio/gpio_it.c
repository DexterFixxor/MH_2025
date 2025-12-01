/*
 * gpio_it.c
 *
 *  Created on: Dec 1, 2025
 *      Author: lazar
 */
#include "stm32f401xe.h"

void
HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == (1 << 13))
	{
		// Toggle PA5
		GPIOA->ODR ^= (1 << 5);
	}
}
