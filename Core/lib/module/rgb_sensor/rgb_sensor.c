/*
 * rgb_sensor.c
 *
 *  Created on: Jan 23, 2026
 *      Author: lazar
 */


#include "rgb_sensor.h"
#include "i2c.h"

uint16_t r = 0, g = 0, b = 0, c = 0;

void rgb_enable()
{
	uint8_t cmd_byte = TCS_CMD_BIT | TCS_ENABLE_REG;
	uint8_t value = TCS_ENABLE_PON;
	HAL_I2C_Mem_Write(&hi2c1, TCS_ADDRESS, cmd_byte, 1, &value, 1, 100);
	HAL_Delay(5);

	value = TCS_ENABLE_PON | TCS_ENABLE_AEN;
	HAL_I2C_Mem_Write(&hi2c1, TCS_ADDRESS, cmd_byte, 1, &value, 1, 100);
	HAL_Delay(3); // po datasheet-u
}

void read_rgbc()
{
	HAL_Delay(50); // integration time senzora

	r = read_16bit(TCS_RED_ADDR);
	b = read_16bit(TCS_BLUE_ADDR);
	g = read_16bit(TCS_GREEN_ADDR);
	c = read_16bit(TCS_CLEAR_ADDR);
}

uint16_t read_16bit(uint8_t reg)
{
	uint8_t cmd_byte = TCS_CMD_BIT | reg;
	HAL_I2C_Master_Transmit(&hi2c1, TCS_ADDRESS, &cmd_byte, 1, 100);

	uint8_t rxBuffer[2] = {0.0};

	HAL_I2C_Master_Receive(&hi2c1, TCS_ADDRESS, rxBuffer, 2, 100);

	return (rxBuffer[1] << 8) | rxBuffer[0];

}
