/*
 * gpio_it.c
 *
 *  Created on: 05.12.2025.
 *      Author: lazar
 */

#include "gpio_it.h"
#include "stm32f4xx.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_13)
	{
		GPIOA->ODR ^= (1 << 5);
	}
}
