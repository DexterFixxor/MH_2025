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

typedef enum
{
	START,
	POS_A,
	AX_100_DEG,
	TIMEOUT_500,
	POS_B,
	AX_0_DEG,
	END
}StrategyStates_t;

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

	StrategyStates_t strategy_state = START;

	strategy_state = POS_A;

	while (1) {

		switch(strategy_state)
		{
		case POS_A:
			set_position_ref(0.7, 0.0, 0.0);
			if (current_motion_state == GOAL_REACHED)
			{
				strategy_state = AX_100_DEG;
				current_motion_state = IDLE;
			}


			break;

//		case AX_100_DEG:
//			ax_goal_position(AX_ID, 100);
//			set_timeout_ms(500);
//			strategy_state = TIMEOUT_500;
//			break;
//
//		case TIMEOUT_500:
//			if (timeout_flags.timeout_end)
//				strategy_state = POS_B;
//			break;
//
//		case POS_B:
//			set_position_ref(0.2, 0.2, M_PI_2);
//			if (current_motion_state == GOAL_REACHED)
//			{
//				strategy_state = AX_0_DEG;
//				current_motion_state = IDLE;
//			}
//			break;
//
//		case AX_0_DEG:
//			ax_goal_position(AX_ID, 0);
//			strategy_state = END;
//			break;
//
//		case END:
//
//			break;
		}

	}
}
