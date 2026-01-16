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

#include "tim.h"
#include "usart.h"
#include <string.h>
#include <math.h>

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

	// Pokreni tajmer da broji
	HAL_TIM_Base_Start_IT(&htim4);

	/* While petlja */
	robot_set_pose_ref(0.3, 0.0, 0.0);

//	ax_goal_position(1, 0);
//	HAL_Delay(500);
//	uint8_t current_strategy_state = 0;
	while (1) {

//		switch(current_strategy_state)
//		{
//		case 0:
//			robot_set_pose_ref(0.2, 0.0, 0.0);
//			current_strategy_state = 1;
//
//			break;
//
//		case 1: // state A
//			if (current_motion_state == GOAL_REACHED)
//			{
//				current_motion_state = IDLE;
//				ax_goal_position(1, 100);
//				current_strategy_state = 2;
//			}
//			break;
//
//		case 2:
//			HAL_Delay(500);
//			current_strategy_state = 3;
//			break;
//
//		case 3:
//			robot_set_pose_ref(0.3, 0.3, M_PI_2);
//			current_strategy_state = 4;
//		}



	}
}
