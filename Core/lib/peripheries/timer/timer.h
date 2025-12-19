/*
 * timer.h
 *
 *  Created on: Dec 5, 2025
 *      Author: lazar
 */

#ifndef LIB_PERIPHERIES_TIMER_TIMER_H_
#define LIB_PERIPHERIES_TIMER_TIMER_H_

#include "stm32f4xx.h"

typedef struct
{
	uint8_t timeout_start : 1;
	uint8_t	timeout_end : 1;
	uint8_t timeout_100s : 1;
	uint8_t timeout_95s : 1;
}FlgTimeouts_TypeDef;


volatile extern FlgTimeouts_TypeDef timeout_flags;

void set_timeout_ms(uint32_t ms);
void delay_ms(uint32_t ms);

#endif /* LIB_PERIPHERIES_TIMER_TIMER_H_ */
