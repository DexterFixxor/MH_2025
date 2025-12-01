/*
 * user_main.c
 *
 *  Created on: Dec 1, 2025
 *      Author: lazar
 */

#include "user_main.h"

void user_init()
{

}

void user_while()
{

	while (1)
	{
		GPIOA->ODR ^= (1 << 5); // togle green LED
		delay_ms(500);
	}
}

