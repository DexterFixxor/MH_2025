/*
 * ax12.c
 *
 *  Created on: Jan 19, 2026
 *      Author: lazar
 */

#include "ax12.h"

void ax_goal_position(uint8_t id, float angle)
{
	uint16_t angle_inc = angle * 1023/300.0;

	uint8_t params[] = {
			AX_GOAL_POSITION,
			(uint8_t)(angle_inc & 0xFF), //donjih 8 bitova se salje prvo
			(uint8_t)(angle_inc >> 8)
	};
	ax_send_msg(id, AX_WRITE, params, 3);
}

void ax_led_state(uint8_t id, uint8_t state)
{
	uint8_t params[] = {AX_LED, state};
	ax_send_msg(id, AX_WRITE, params, 2);
}

void ax_send_msg(uint8_t id, uint8_t instruction, uint8_t *params, uint8_t n_params)
{
	uint8_t len = n_params + 2;

	uint16_t chksum = 0;
	chksum = id + len + instruction;

	for (int i = 0; i < n_params; i++)
		chksum += params[i];

	ax_send_byte(0xFF); // HEADER
	ax_send_byte(0xFF); // HEADER
	ax_send_byte(id);	// ID
	ax_send_byte(len);	// Length
	ax_send_byte(instruction);
	ax_send_byte_array(params, n_params);
	ax_send_byte((uint8_t)(~chksum & 0xFF));
}


void ax_send_byte(uint8_t data)
{
	while ((USART1->SR & (0b1 << 6)) == 0); // Transmission complete
	USART1->DR = data;
}

void ax_send_byte_array(uint8_t *data, uint8_t n_data)
{
	for (int i = 0; i < n_data; i ++)
	{
		ax_send_byte(data[i]);
	}
}
