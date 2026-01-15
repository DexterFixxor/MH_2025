/*
 * ax12.h
 *
 *  Created on: Jan 15, 2026
 *      Author: lazar
 */

#ifndef LIB_MODULE_AX12_AX12_H_
#define LIB_MODULE_AX12_AX12_H_

#include "stm32f4xx.h"

// instructions
#define AX_PING 	0x01
#define AX_READ 	0x02
#define AX_WRITE 	0x03

// parameters
#define AX_LED				25
#define AX_GOAL_POSITION 	30
#define AX_MOVING_SPEED		32
#define AX12_ID 			1

void ax_goal_position(uint8_t id, float angle);
void ax_led(uint8_t id, uint8_t state);
void ax_send_msg(uint8_t id, uint8_t instruction, uint8_t* parameters, uint8_t n_params);

#endif /* LIB_MODULE_AX12_AX12_H_ */
