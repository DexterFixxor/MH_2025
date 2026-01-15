/*
 * ax12.c
 *
 *  Created on: Jan 15, 2026
 *      Author: lazar
 */

#include "ax12.h"
#include "usart.h"


void ax_goal_position(uint8_t id, float angle)
{
	if (angle > 300)
		angle = 300.0;
	else if (angle < 0)
		angle = 0.0;

	uint16_t angle_inc = (uint16_t)(angle * 1023/300.0); // [0-1023]

	uint8_t params[] = {
			AX_GOAL_POSITION,
			(uint8_t)(angle_inc & 0xFF),
			(uint8_t)((angle_inc >> 8) & 0xFF),
	};

	ax_send_msg(id, AX_WRITE, params, 3);
}

void ax_led(uint8_t id, uint8_t state)
{
	if (state > 1)
		state = 1;

	uint8_t params[] = {
			AX_LED,
			state
	};
	ax_send_msg(id, AX_WRITE, params, 2);
}

void ax_send_msg(uint8_t id, uint8_t instruction, uint8_t* parameters, uint8_t n_params)
{
	uint8_t header[] = {0xFF, 0xFF};
	uint8_t len = n_params + 2;
	uint16_t checksum_tmp = 0;

	checksum_tmp = id + len + instruction;

	for (int i = 0; i < n_params; i++)
	{
		checksum_tmp += parameters[i];
	}

	uint8_t chksum = 0xFF & (~checksum_tmp);

	// posalji 2x header
	HAL_UART_Transmit(&huart1, header , 2, HAL_MAX_DELAY);

	// posalji ID
	HAL_UART_Transmit(&huart1, &id, 1, HAL_MAX_DELAY);

	// duzina
	HAL_UART_Transmit(&huart1, &len, 1, HAL_MAX_DELAY);

	HAL_UART_Transmit(&huart1, &instruction, 1, HAL_MAX_DELAY);

	HAL_UART_Transmit(&huart1, parameters, n_params, HAL_MAX_DELAY);

	HAL_UART_Transmit(&huart1, &chksum, 1, HAL_MAX_DELAY);
}
