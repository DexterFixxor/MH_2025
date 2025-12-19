/*
 * user_main.c
 *
 *  Created on: Dec 5, 2025
 *      Author: lazar
 */

#include "stm32f4xx.h"
#include "user_main.h"
#include "peripheries/timer/timer.h"
#include "peripheries/gpio/gpio_it.h"
#include "tim.h"
#include "usart.h"
#include <string.h>

typedef enum
{
	NO_BLINK,
	BLINK100MS,
	BLINK500MS,
	BLINK1000MS,
}FSM_States_TypeDef;

	// Enable encoder interface on tim2 & tim3
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	encoders_init();

void
user_main()
{
	/* Init */
	FSM_States_TypeDef current_state = NO_BLINK;
	uint8_t init_state = 0;

	// Pokreni tajmer da broji
	HAL_TIM_Base_Start_IT(&htim4);

	/* While petlja */

	uint8_t msg[] = "Please send dt for blink!\n\r";
	HAL_UART_Transmit(&huart2, msg, strlen(msg), HAL_MAX_DELAY);

	uint8_t data = 10;

	set_timeout_ms(data * 10);
	while (1)
	{

		HAL_UART_Receive(&huart2, &data, 1, 5);

		if (timeout_flags.timeout_end)
		{
			GPIOA->ODR ^= (1 << 5);
			set_timeout_ms(data * 10);
		}

//		switch (current_state) {
//		case NO_BLINK:
//			/* Init */
//
//			/* Telo */
//
//			/* Provera uslova prelaska */
//			if (flg_blue_btn_pressed)
//			{
//				current_state = BLINK100MS;
//				flg_blue_btn_pressed = 0;
//				init_state = 1;
//			}
//			break;
//		case BLINK100MS:
//			/* Init */
//			if (init_state)
//			{
//				HAL_UART_Transmit(&huart2, (uint8_t*)"\n\rInit blink on 100ms\n\r", 23, HAL_MAX_DELAY);
//				set_timeout_ms(100);
//				init_state = 0;
//			}
//			/* Telo */
//			if (timeout_flags.timeout_end)
//			{
//				GPIOA->ODR ^= (1 << 5);
//				set_timeout_ms(100);
//			}
//			/* Provera uslova prelaska */
//			if (flg_blue_btn_pressed)
//			{
//				flg_blue_btn_pressed = 0;
//				current_state = BLINK500MS;
//				init_state = 1;
//			}
//			break;
//
//		case BLINK500MS:
//			/* Init */
//			if (init_state)
//			{
//				HAL_UART_Transmit(&huart2, (uint8_t*)"\n\rInit blink on 500ms\n\r", 23, HAL_MAX_DELAY);
//				set_timeout_ms(500);
//				init_state = 0;
//			}
//			/* Telo */
//			if (timeout_flags.timeout_end)
//			{
//				GPIOA->ODR ^= (1 << 5);
//				set_timeout_ms(500);
//			}
//
//			/* Provera uslova prelaska */
//			if (flg_blue_btn_pressed)
//			{
//				flg_blue_btn_pressed = 0;
//				current_state = BLINK1000MS;
//				init_state = 1;
//			}
//			break;
//
//		case BLINK1000MS:
//			/* Init */
//			if (init_state)
//			{
//				HAL_UART_Transmit(&huart2, (uint8_t*)"\n\rInit blink on 1000ms\n\r", 23, HAL_MAX_DELAY);
//				set_timeout_ms(1000);
//				init_state = 0;
//			}
//			/* Telo */
//			if (timeout_flags.timeout_end)
//			{
//				GPIOA->ODR ^= (1 << 5);
//				set_timeout_ms(1000);
//			}
//
//			/* Provera uslova prelaska */
//			if (flg_blue_btn_pressed)
//			{
//				flg_blue_btn_pressed = 0;
//				current_state = BLINK100MS;
//				init_state = 1;
//			}
//			break;
//		}


	}
}
