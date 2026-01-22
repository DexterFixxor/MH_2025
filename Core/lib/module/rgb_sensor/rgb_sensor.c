/*
 * rgb_sensor.c
 *
 *  Created on: Jan 22, 2026
 *      Author: lazar
 */

#include "rgb_sensor.h"

void write8_bit(uint8_t reg, uint8_t value)
{
	uint8_t txBuffer[2] = {
			RGB_COMMAND_BIT | reg,
			value
	};

	HAL_I2C_Master_Transmit(&hi2c1, RGB_SENSOR_ADDR, txBuffer, 2, 100);
}

uint16_t read16_bit(uint8_t reg)
{
	uint8_t txBuffer[] = { RGB_COMMAND_BIT | reg };
	uint8_t rxBuffer[2] = {0};

	HAL_I2C_Master_Transmit(&hi2c1, RGB_SENSOR_ADDR, txBuffer, 1, 100);
	HAL_I2C_Master_Receive(&hi2c1, RGB_SENSOR_ADDR, rxBuffer, 2, 100);

	return (uint16_t)((rxBuffer[1] << 8) | rxBuffer[0]);
}

void enable_rgb_sensor()
{
	// Power ON
	write8_bit(RGB_ENABLE, ENABLE_PON);
	HAL_Delay(10);

	// ADC Enable (RGBC enable)
	write8_bit(RGB_ENABLE, ENABLE_PON | ENABLE_AEN);
	HAL_Delay(5);
}


void getRawData(uint16_t* red, uint16_t* green, uint16_t* blue, uint16_t* clear)
{
	HAL_Delay(50); // integration time

	*red = read16_bit(RED_ADDR);
	*green = read16_bit(GREEN_ADDR);
	*blue = read16_bit(BLUE_ADDR);
	*clear = read16_bit(CLEAR_ADDR);
}

void getRGB(uint8_t* r, uint8_t* g, uint8_t* b)
{
	uint16_t red = 0, green = 0, blue = 0, clear = 0;
	getRawData(&red, &green, &blue, &clear);

	*r = 255 * ((float)red / clear);
	*b = 255 * ((float)blue / clear);
	*g = 255 * ((float)green / clear);


}
