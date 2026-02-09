/*
 * cmd_parser.c
 *
 *  Created on: Feb 9, 2026
 *      Author: lazar
 */
#include "cmd_parser.h"
#include "module/odom/odom.h"
#include "usart.h"

// len - broj bajtova nakon 'len'
// [0xFF, 0xFF, len, cmd, param1, ... param n, CHKSUM]
// CHKSUM = ~(cmd + sum(params)) & 0xFF
uint8_t cmd_parser_buffer[CMD_PARSER_BUFFER_SIZE][CMD_PARSER_MAX_MSG_SIZE] = {0};
uint16_t cmd_parser_read = 0;
uint16_t cmd_parser_write = 0;

cmd_callback_fn cmd_table[__MAX_CMD_ID] = {NULL};

uint8_t msg_to_send[32] = {0};

void increment_read();

// prototipovi CMD funkcija
void toggle_led(uint8_t* msg);
void send2floats(uint8_t* msg);
void odom_req(uint8_t* msg);

void init_cmd_table()
{
	cmd_table[TOGGLE_LED] = toggle_led;
	cmd_table[SEND2FLOATS] = send2floats;
	cmd_table[ODOM_REQ] = odom_req;
}

void process_msg()
{
	// Nema novih poruka
	if (cmd_parser_read == cmd_parser_write)
		return;

	uint8_t* msg = cmd_parser_buffer[cmd_parser_read];

	// Provera header-a
	if (msg[0] != 0xFF || msg[1] != 0xFF)
	{
		increment_read();
		return;
	}

	uint8_t len = msg[2];
	uint8_t cmd = msg[3];

	uint16_t chksum = 0;

	for (int i = 3; i < len + 2; i++)
	{
		chksum += msg[i];
	}

	chksum = (~chksum) & 0xFF;

	// provera CHKSUM
	if (chksum != msg[len+2])
	{
		increment_read();
		return;
	}

	if (cmd < __MAX_CMD_ID && cmd_table[cmd] != NULL)
	{
		cmd_table[cmd](msg);
	}

	increment_read();
}

void increment_read()
{
	if(++cmd_parser_read == CMD_PARSER_BUFFER_SIZE)
	{
		cmd_parser_read = 0;
	}
}

void toggle_led(uint8_t* msg)
{
	GPIOA->ODR ^= 1 << 5;
}

void send2floats(uint8_t* msg)
{
	uint8_t a_offset = 4;
	uint8_t b_offset = a_offset + 4;

	uinon_f_2_uint_t converter;

	converter.ui[0] = msg[a_offset];
	converter.ui[1] = msg[a_offset + 1];
	converter.ui[2] = msg[a_offset + 2];
	converter.ui[3] = msg[a_offset + 3];

	float a = converter.f;


	converter.ui[0] = msg[b_offset];
	converter.ui[1] = msg[b_offset + 1];
	converter.ui[2] = msg[b_offset + 2];
	converter.ui[3] = msg[b_offset + 3];

	float b = converter.f;

}

void odom_req(uint8_t* msg)
{
	uinon_f_2_uint_t x_c, y_c, theta_c;
	uint16_t chksum = 0;

	x_c.f = x;
	y_c.f = y;
	theta_c.f = theta;

	chksum = ODOM_REQ;

	for (int i = 0; i < 4; i++)
	{
		chksum += (x_c.ui[i] + y_c.ui[i] + theta_c.ui[i]);
	}

	chksum = ~chksum & 0xFF;

	uint8_t tmp[] = {
			0xFF,
			0xFF,
			14, // LEN
			ODOM_REQ, // CMD

			x_c.ui[0],
			x_c.ui[1],
			x_c.ui[2],
			x_c.ui[3],

			y_c.ui[0],
			y_c.ui[1],
			y_c.ui[2],
			y_c.ui[3],

			theta_c.ui[0],
			theta_c.ui[1],
			theta_c.ui[2],
			theta_c.ui[3],

			(uint8_t)chksum
	};

	for (int i = 0; i < 17; i ++)
	{
		msg_to_send[i] = tmp[i];
	}

	HAL_UART_Transmit_DMA(&huart2, msg_to_send, 17);
}
