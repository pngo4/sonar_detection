/*
 * ili9486.h
 *
 *  Created on: Jul 30, 2025
 *      Author: paul
 */

#ifndef INC_ILI9486_H_
#define INC_ILI9486_H_

#include <stdint.h>

// Basic init
void ili9486_init(void);

// Core drawing routines
void draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void fill_screen(uint16_t color);
void lcd_printf(int x, int y, const char* str, uint16_t color);



#endif /* INC_ILI9486_H_ */
