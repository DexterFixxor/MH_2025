/*
 * cmd_parser.h
 *
 *  Created on: Feb 6, 2026
 *      Author: lazar
 */

#ifndef LIB_MODULE_CMD_PARSER_CMD_PARSER_H_
#define LIB_MODULE_CMD_PARSER_CMD_PARSER_H_

#include "stm32f4xx.h"

#define PARSER_CMD_BUFFER 	32
#define PARSER_MSG_LEN		32

typedef enum
{
	TOGGLE_LED,
	MAX_INSTRUCTION_ID // OVA SE NE KORISTI
}uart_cmd_t;

typedef void (*callback_cmd_t) (uint8_t* msg);

extern uint8_t cmd_buffer[PARSER_CMD_BUFFER][PARSER_MSG_LEN];
extern uint16_t cmd_buffer_read;
extern uint16_t cmd_buffer_write;


void init_cmd_table();
void parse_cmd();

void toggle_led();

#endif /* LIB_MODULE_CMD_PARSER_CMD_PARSER_H_ */
