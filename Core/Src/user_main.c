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
#include "module/odom/odom.h"
#include "module/position/position.h"
#include "module/ax12/ax12.h"
#include "module/rgb_sensor/rgb_sensor.h"
#include "peripheries/dma_uart/dma_uart.h"

#include "dma.h"
#include "tim.h"
#include "usart.h"
#include <string.h>
#include <math.h>

void user_main() {
	/* Init */

	// Enable encoder interface on tim2 & tim3
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	encoders_init();

	// Enable PWM on Motor1 & Motor2
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

	// Pokreni tajmer da broji
	HAL_TIM_Base_Start_IT(&htim4);

	//Enable prekid na idle stanje
//	UART2->CR1 |= (1 << 4);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart2, dma_rx_buffer, DMA_RX_BUFFER_SIZE);
	init_cmd_table();

	rgb_sensor_enable();
	/* While petlja */

	while (1) {
		process_msg();
	}
}
