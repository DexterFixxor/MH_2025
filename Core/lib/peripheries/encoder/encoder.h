/*
 * encoder.h
 *
 *  Created on: Dec 19, 2025
 *      Author: lazar
 */

#ifndef LIB_PERIPHERIES_ENCODER_ENCODER_H_
#define LIB_PERIPHERIES_ENCODER_ENCODER_H_

#include "stm32f4xx.h"

#define ENC_START_INC (0xFFFF >> 1)

void encoders_init();

int32_t enc1_get_delta_inc();
int32_t enc2_get_delta_inc();

#endif /* LIB_PERIPHERIES_ENCODER_ENCODER_H_ */
