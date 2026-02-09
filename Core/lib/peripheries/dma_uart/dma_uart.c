/*
 * dma_uart.c
 *
 *  Created on: Feb 9, 2026
 *      Author: lazar
 */

#include "dma_uart.h"
#include "module/cmd_parser/cmd_parser.h"
#include "usart.h"
#include <memory.h>

uint8_t dma_rx_buffer[DMA_RX_BUFFER_SIZE] = {0};


void uart_dma_callback()
{
	static uint16_t prev_pos;
	uint16_t current_pos;
	uint16_t to_copy = 0;

	current_pos = DMA_RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart2.hdmarx);

	if (current_pos != prev_pos)
	{
		if (current_pos > prev_pos)
		{
			to_copy = current_pos - prev_pos;
			// ovde kopiramo u privremeni niz
			memcpy(cmd_parser_buffer[cmd_parser_write], &dma_rx_buffer[prev_pos], to_copy);
		}
		else
		{
			to_copy = DMA_RX_BUFFER_SIZE - prev_pos;
			memcpy(cmd_parser_buffer[cmd_parser_write], &dma_rx_buffer[prev_pos], to_copy);
			if (current_pos > 0)
			{
				memcpy(&cmd_parser_buffer[cmd_parser_write][to_copy], &dma_rx_buffer[0], current_pos);
			}
		}

		if(++cmd_parser_write == CMD_PARSER_BUFFER_SIZE)
			cmd_parser_write = 0;
	}

	prev_pos = current_pos;
}
