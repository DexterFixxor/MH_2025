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
#include "peripheries/encoder/encoder.h"
#include "module/bdc_motor/bdc_motor.h"
#include "module/position/position.h"
#include "module/odom/odom.h"
#include "module/ax12/ax12.h"
#include "module/rgb_sensor/rgb_sensor.h"
#include "peripheries/uart_dma/uart_dma.h"
#include "module/cmd_parser/cmd_parser.h"

#include "tim.h"
#include "usart.h"
#include <string.h>
#include <math.h>
#include "stm32f4xx_hal_uart.h"


typedef enum {
	NO_BLINK, BLINK100MS, BLINK500MS, BLINK1000MS,
} FSM_States_TypeDef;

void user_main() {
	/* Init */

	// Enable encoder interface on tim2 & tim3
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	encoders_init();

	// Enable PWM on Motor1 & Motor2
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

	// Enable UART IDLE interrupt
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart2, dma_rx_buffer, DMA_BUFFER_SIZE);
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);

	// Pokreni tajmer da broji
	HAL_TIM_Base_Start_IT(&htim4);
	rgb_enable();

	init_cmd_table();

	/* While petlja */
	while (1) {
		parse_cmd();
	}
}
