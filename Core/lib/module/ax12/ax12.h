/*
 * ax12.h
 *
 *  Created on: Jan 19, 2026
 *      Author: lazar
 */

#ifndef LIB_MODULE_AX12_AX12_H_
#define LIB_MODULE_AX12_AX12_H_

// instructions
#define AX_READ 	0x02
#define AX_WRITE 	0x03

// control table
#define AX_LED				25
#define AX_GOAL_POSITION 	30

//ids
#define AX_ID		1

#include "stm32f4xx.h"

void ax_goal_position(uint8_t id, float angle);
void ax_led_state(uint8_t id, uint8_t state);
void ax_send_msg(uint8_t id, uint8_t instruction, uint8_t *params, uint8_t n_params);

void ax_send_byte(uint8_t data);
void ax_send_byte_array(uint8_t *data, uint8_t n_data);

#endif /* LIB_MODULE_AX12_AX12_H_ */
