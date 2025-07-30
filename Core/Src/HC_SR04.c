/*
 * HC_SR04.c
 *
 *  Created on: Jul 29, 2025
 *      Author: paul
 */
#include "stm32f4xx_hal.h"
#include "HC_SR04.h"
#include <string.h>

#define TRIG_PIN GPIO_PIN_12
#define TRIG_PORT GPIOA
#define ECHO_PIN GPIO_PIN_11
#define ECHO_PORT GPIOA
#define MEDIAN_FILTER_SIZE 7

uint32_t dist_buffer[MEDIAN_FILTER_SIZE];
uint8_t dist_index = 0;
uint8_t buffer_filled = 0;

uint32_t store_millis;
uint32_t value1 = 0; //first measured value
uint32_t value2 = 0; //second measured value
uint16_t distance = 0; //in cm

uint32_t get_median(uint32_t* values, uint8_t size) {
	uint32_t sorted[size];
	memcpy(sorted, values, size * sizeof(uint32_t));

	//bubble sort

	for(int i = 0; i < size - 1; i++) {
		for (int j = i + 1; j < size; j++) {
			if(sorted[i] > sorted[j]) {
				uint32_t tmp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = tmp;
			}
		}
	}
	return sorted[size/2]; // median
}

uint32_t median_filter(uint32_t new_value) {

	//reject bad data, spec for HCSR04 is around 20 cm to 400 cm
	if(new_value < 5 || new_value > 400) {
		return get_median(dist_buffer, buffer_filled ? MEDIAN_FILTER_SIZE : dist_index);
	}

	dist_buffer[dist_index++] = new_value;
	if(dist_index >= MEDIAN_FILTER_SIZE) {
		dist_index = 0;
		buffer_filled = 1;
	}

	return get_median(dist_buffer, buffer_filled ? MEDIAN_FILTER_SIZE : dist_index);
}

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

