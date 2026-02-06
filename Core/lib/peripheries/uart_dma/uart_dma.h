/*
 * uart_dma.h
 *
 *  Created on: Feb 6, 2026
 *      Author: lazar
 */

#ifndef LIB_PERIPHERIES_UART_DMA_UART_DMA_H_
#define LIB_PERIPHERIES_UART_DMA_UART_DMA_H_

#include "stm32f4xx.h"

#define DMA_BUFFER_SIZE 256


extern uint8_t dma_rx_buffer[DMA_BUFFER_SIZE];


void uart_idle_callback();

#endif /* LIB_PERIPHERIES_UART_DMA_UART_DMA_H_ */
