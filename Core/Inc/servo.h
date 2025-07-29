/*
 * servo.h
 *
 *  Created on: Jul 29, 2025
 *      Author: paul
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

void set_servo_angle(TIM_HandleTypeDef *htim, uint32_t channel, uint8_t angle);

#endif /* INC_SERVO_H_ */
