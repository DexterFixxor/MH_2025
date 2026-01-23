/*
 * rgb_sensor.h
 *
 *  Created on: Jan 23, 2026
 *      Author: lazar
 */

#ifndef LIB_MODULE_RGB_SENSOR_RGB_SENSOR_H_
#define LIB_MODULE_RGB_SENSOR_RGB_SENSOR_H_

#include "stm32f4xx.h"

#define TCS_ADDRESS (0x29 << 1)

#define TCS_CMD_BIT (0x80)

#define TCS_ENABLE_REG	(0x00)

#define TCS_ENABLE_PON	(0b1 << 0)
#define TCS_ENABLE_AEN	(0b1 << 1)

#define TCS_CLEAR_ADDR	(0x14)
#define TCS_RED_ADDR	(0x16)
#define TCS_GREEN_ADDR	(0x18)
#define TCS_BLUE_ADDR	(0x1A)

extern uint16_t r, g, b, c;

void rgb_enable();
void read_rgbc();
uint16_t read_16bit(uint8_t reg);
#endif /* LIB_MODULE_RGB_SENSOR_RGB_SENSOR_H_ */
