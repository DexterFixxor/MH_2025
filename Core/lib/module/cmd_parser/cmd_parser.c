/*
 * cmd_parser.c
 *
 *  Created on: Feb 6, 2026
 *      Author: lazar
 */

#include "cmd_parser.h"

//   0     1     2    3     4             4 + N,   4 + N + 1
// [0xFF, 0xFF, LEN, CMD, PARAM 1, ..., PARAM N, CHKSUM]
// chksum = ~(cmd + param 1 + ... + param n) & 0xFF
uint8_t cmd_buffer[PARSER_CMD_BUFFER][PARSER_MSG_LEN] = {0};
uint16_t cmd_buffer_read = 0;
uint16_t cmd_buffer_write = 0;

callback_cmd_t cmd_table[MAX_INSTRUCTION_ID] = {NULL};


void init_cmd_table()
{
	cmd_table[TOGGLE_LED] = toggle_led;
}


void toggle_led(uint8_t* msg)
{
	GPIOA->ODR ^= 1 << 5;
}

void increment_read()
{
	if (++cmd_buffer_read == PARSER_CMD_BUFFER)
		cmd_buffer_read = 0;

	// cmd_buffer_read = (cmd_buffer_read + 1) % PARSER_CMD_BUFFER;
}

void parse_cmd()
{
	if (cmd_buffer_read == cmd_buffer_write)
		return;

	uint8_t *msg = cmd_buffer[cmd_buffer_read];

	// check HEADER
	if (msg[0] != 0xFF || msg[1] != 0xFF)
	{
		increment_read();
		return;
	}


	uint16_t len = msg[2];
	uint16_t instruction = msg[3];

	uint16_t chksum = 0;
	for (int i = 3; i < len + 2; i++)
	{
		chksum += msg[i];
	}
	chksum = (~chksum) & 0xFF;

	// check CHKSUM
	if (chksum != msg[len+2])
	{
		increment_read();
		return;
	}

	//	 check INSTRUCTION
	if (instruction < MAX_INSTRUCTION_ID && cmd_table[instruction] != NULL)
	{
		cmd_table[instruction](msg);
	}

	increment_read();
}
