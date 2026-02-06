/*
 * uart_dma.c
 *
 *  Created on: Feb 6, 2026
 *      Author: lazar
 */

#include "uart_dma.h"
#include "usart.h"
#include "dma.h"
#include <memory.h>

#include "module/cmd_parser/cmd_parser.h"


uint8_t dma_rx_buffer[DMA_BUFFER_SIZE] = {0};

void uart_idle_callback()
{
	static uint32_t prev_pos = 0;
	uint32_t current_pos;
	uint32_t to_copy;

	current_pos = DMA_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart2.hdmarx);

	if (current_pos != prev_pos)
	{
		if (current_pos > prev_pos)
		{
			to_copy = current_pos - prev_pos;
			// memcopy
			memcpy(cmd_buffer[cmd_buffer_write], &dma_rx_buffer[prev_pos], to_copy);
		}
		else
		{
			to_copy = DMA_BUFFER_SIZE - prev_pos;
			memcpy(cmd_buffer[cmd_buffer_write], &dma_rx_buffer[prev_pos], to_copy);
			if(current_pos > 0)
			{
				memcpy(&cmd_buffer[cmd_buffer_write][to_copy], &dma_rx_buffer[0], current_pos);
			}
		}

		prev_pos = current_pos;

		if (prev_pos == DMA_BUFFER_SIZE)
			prev_pos = 0;

		cmd_buffer_write += 1;
		if (cmd_buffer_write == PARSER_CMD_BUFFER)
			cmd_buffer_write = 0;
	}
}

void USART2_IRQHandler()
{
	if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE))
	{
		__HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_IDLE);
		uart_idle_callback();
	}
}
