/*
 * dma_uart.h
 *
 *  Created on: Feb 9, 2026
 *      Author: lazar
 */

#ifndef LIB_PERIPHERIES_DMA_UART_DMA_UART_H_
#define LIB_PERIPHERIES_DMA_UART_DMA_UART_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_uart.h"

#define DMA_RX_BUFFER_SIZE 64

uint8_t dma_rx_buffer[DMA_RX_BUFFER_SIZE];


void uart_dma_callback();

#endif /* LIB_PERIPHERIES_DMA_UART_DMA_UART_H_ */
