/*
 * HC_SR04.h
 *
 *  Created on: Jul 29, 2025
 *      Author: paul
 */

#ifndef INC_HC_SR04_H_
#define INC_HC_SR04_H_


void HCR04_init(TIM_HandleTypeDef *tim);
uint16_t HCSR04_read(TIM_HandleTypeDef *tim);
uint32_t median_filter(uint32_t new_value);

#endif /* INC_HC_SR04_H_ */
