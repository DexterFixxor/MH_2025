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

	Pose_t pos1 = {
			.x = 0.3,
			.y = 0,
			.theta = 0
	};

	Pose_t pos2 = {
			.x = 0.3,
			.y = 0.3,
			.theta = M_PI_2
	};

	uint16_t strategy_state = 0;
	ax_goal_position(AX12_ID, 0.0);
	HAL_Delay(500);
	while (1) {

		switch(strategy_state)
		{
		case 0: // faza mirovanja
			set_ref_pose(pos1.x, pos1.y, pos1.theta);
			strategy_state = 1;
			break;


		case 1: // faza kretanja, cekam da robot stigne do ref tacke

			if (current_motion_state == GOAL_REACHED)
			{
				current_motion_state = IDLE;
				strategy_state = 2;
			}
			break;
//		case 2: //cekaj timeout
//			strategy_state = 3;
//			break;
//
//		case 3: // pomeri AX
//			ax_goal_position(AX12_ID, 150.0);
//			strategy_state = 4;
//			break;
//
//		case 4: //cekaj da se ax zarotira
//			// uradite sami
//			strategy_state = 5;
//			break;
//
//		case 5:
//			set_ref_pose(pos2.x, pos2.y, pos2.theta);
//			strategy_state = 6;
//			break;
//		default:
//			break;
		}
	}
}
