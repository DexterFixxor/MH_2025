/*
 * timer.h
 *
 *  Created on: Dec 1, 2025
 *      Author: lazar
 */

#ifndef LIB_PERIPHERIES_TIMER_TIMER_H_
#define LIB_PERIPHERIES_TIMER_TIMER_H_

#include "stm32f401xe.h"

typedef struct Flg_Timeout
{
	unsigned char flg_100s : 1;
	unsigned char flg_1s : 1;
	unsigned char flg_timeout_start : 1;
	unsigned char flg_timeout_end : 1;
}Flg_Timeout_t;

extern Flg_Timeout_t flg_timeouts;
void timeout_ms_function(uint32_t ms);
void delay_ms(uint32_t ms);

#endif /* LIB_PERIPHERIES_TIMER_TIMER_H_ */
