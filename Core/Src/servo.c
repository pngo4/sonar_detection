/*
 * servo.c
 *
 *  Created on: Jul 29, 2025
 *      Author: paul
 */
#include "stm32f4xx_hal.h"
#include "servo.h"

void set_servo_angle(TIM_HandleTypeDef *htim, uint32_t channel, uint8_t angle)
{
	//map angle (0-180) to pulse width (210-1050 counts)
	uint32_t pulse_length = 250 + (angle * (1100 - 250) / 180);
	__HAL_TIM_SET_COMPARE(htim, channel, pulse_length);
}
