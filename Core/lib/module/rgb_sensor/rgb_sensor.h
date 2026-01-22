/*
 * rgb_sensor.h
 *
 *  Created on: Jan 22, 2026
 *      Author: lazar
 */

#ifndef LIB_MODULE_RGB_SENSOR_RGB_SENSOR_H_
#define LIB_MODULE_RGB_SENSOR_RGB_SENSOR_H_

#include "i2c.h"

#define RGB_SENSOR_ADDR		(0x29 << 1)
#define RGB_COMMAND_BIT		(0x80)

/* Control registers */
#define RGB_ENABLE			(0x00)

#define ENABLE_PON			(0x01)
#define ENABLE_AEN			(0x02)


#define CLEAR_ADDR			(0x14)
#define RED_ADDR			(0x16)
#define GREEN_ADDR			(0x18)
#define BLUE_ADDR			(0x1A)

void write8_bit(uint8_t reg, uint8_t value);
uint16_t read16_bit(uint8_t reg);

void enable_rgb_sensor();
void getRawData(uint16_t* red, uint16_t* green, uint16_t* blue, uint16_t* clear);
void getRGB(uint8_t* r, uint8_t* g, uint8_t* b);
#endif /* LIB_MODULE_RGB_SENSOR_RGB_SENSOR_H_ */
