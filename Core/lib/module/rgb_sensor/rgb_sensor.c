/*
 * rgb_sensor.c
 *
 *  Created on: Jan 26, 2026
 *      Author: lazar
 */

#include "rgb_sensor.h"

uint16_t r = 0, g = 0, b = 0, c = 0;
uint8_t red = 0, green = 0, blue = 0;

static uint16_t read16_bit(uint8_t reg);

void read_rgbc()
{
	HAL_Delay(50);
	r = read16_bit(TCS_RED_ADDR);
	g = read16_bit(TCS_GREEN_ADDR);
	b = read16_bit(TCS_BLUE_ADDR);
	c = read16_bit(TCS_CLEAR_ADDR);

	if (c > 0)
	{
		red = 255 * ((float)r / c);
		blue = 255 * ((float)b / c);
		green = 255 * ((float)g / c);
	}
}

void rgb_sensor_enable()
{
	uint8_t enable_cmd[] = {
			TCS_COMMAND_BIT | TCS_ENABLE_REG,
			(0b1 << 0) // Power ON
	};
	HAL_I2C_Master_Transmit(&hi2c1, TCS_ID, enable_cmd, 2, 100);

	HAL_Delay(5);

	uint8_t rgbc_enable_cmd[] = {
			TCS_COMMAND_BIT | TCS_ENABLE_REG,
			(0b11) // Power ON & ADC Enable
	};
	HAL_I2C_Master_Transmit(&hi2c1, TCS_ID, rgbc_enable_cmd, 2, 100);

	HAL_Delay(50);
}

static uint16_t read16_bit(uint8_t reg)
{
	uint8_t readCmd[] = {
			TCS_COMMAND_BIT | reg
	};

	HAL_I2C_Master_Transmit(&hi2c1, TCS_ID, readCmd, 1, 100);

	uint8_t rxBuffer[2] = {0};
	HAL_I2C_Master_Receive(&hi2c1, TCS_ID, rxBuffer, 2, 100);

	return (rxBuffer[1] << 8) | rxBuffer[0];

}
