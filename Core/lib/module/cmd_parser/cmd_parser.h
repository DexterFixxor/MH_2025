/*
 * cmd_parser.h
 *
 *  Created on: Feb 9, 2026
 *      Author: lazar
 */

#ifndef LIB_MODULE_CMD_PARSER_CMD_PARSER_H_
#define LIB_MODULE_CMD_PARSER_CMD_PARSER_H_

#include "stm32f4xx.h"

#define CMD_PARSER_BUFFER_SIZE		32
#define CMD_PARSER_MAX_MSG_SIZE		32

typedef enum
{
	TOGGLE_LED,
	SEND2FLOATS,
	ODOM_REQ,
	__MAX_CMD_ID
}uart_cmd_t;

typedef union
{
	float f;
	uint8_t ui[4];
}uinon_f_2_uint_t;

extern uint8_t cmd_parser_buffer[CMD_PARSER_BUFFER_SIZE][CMD_PARSER_MAX_MSG_SIZE];
extern uint16_t cmd_parser_read;
extern uint16_t cmd_parser_write;

void init_cmd_table();
void process_msg();

typedef void (*cmd_callback_fn)(uint8_t* msg);

#endif /* LIB_MODULE_CMD_PARSER_CMD_PARSER_H_ */
