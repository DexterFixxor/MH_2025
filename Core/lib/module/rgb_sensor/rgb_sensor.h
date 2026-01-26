/*
 * rgb_sensor.h
 *
 *  Created on: Jan 26, 2026
 *      Author: lazar
 */

#ifndef LIB_MODULE_RGB_SENSOR_RGB_SENSOR_H_
#define LIB_MODULE_RGB_SENSOR_RGB_SENSOR_H_

#include "i2c.h"

#define TCS_ID				(0x29 << 1)
#define TCS_COMMAND_BIT		(0b1 << 7)
#define TCS_ENABLE_REG		(0x00)
#define TCS_CLEAR_ADDR		(0x14)
#define TCS_RED_ADDR		(0x16)
#define TCS_GREEN_ADDR		(0x18)
#define TCS_BLUE_ADDR		(0x1A)


extern uint16_t r, g, b, c;

void rgb_sensor_enable();
void read_rgbc();

#endif /* LIB_MODULE_RGB_SENSOR_RGB_SENSOR_H_ */
