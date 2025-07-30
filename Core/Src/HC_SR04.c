/*
 * HC_SR04.c
 *
 *  Created on: Jul 29, 2025
 *      Author: paul
 */
#include "stm32f4xx_hal.h"
#include "HC_SR04.h"

#define TRIG_PIN GPIO_PIN_12
#define TRIG_PORT GPIOA
#define ECHO_PIN GPIO_PIN_11
#define ECHO_PORT GPIOA


uint32_t store_millis;
uint32_t value1 = 0; //first measured value
uint32_t value2 = 0; //second measured value
uint16_t distance = 0; //in cm

void HCR04_init(TIM_HandleTypeDef *tim) {
	HAL_TIM_Base_Start(tim);
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
}


uint16_t HCSR04_read(TIM_HandleTypeDef *tim) {

	//set for 10 uS
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
	__HAL_TIM_SET_COUNTER(tim, 0);
	while(__HAL_TIM_GET_COUNTER(tim) < 10); //wait for 10 uS
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET); //set back to low

	//now reading starts
	store_millis = HAL_GetTick(); //use tick to avoid infinite while loop, stores current time
	//wait for echo pin to go HIGH
	while(HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_RESET) {
		if((HAL_GetTick() - store_millis) > 100) return 0; //timeout
	}

	value1 = __HAL_TIM_GET_COUNTER(tim);

	store_millis = HAL_GetTick();//stores current time
	while(HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_SET) {
		if((HAL_GetTick() - store_millis) > 80) return 0; // timeout
	}
	value2 = __HAL_TIM_GET_COUNTER(tim);

	// distance = (value2 - value1) / 58;
	distance = (value2 - value1) / 58;

	HAL_Delay(50);

	return distance;
}

