/*
 * display.h
 *
 *  Created on: Jan 20, 2026
 *      Author: dwask
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include <stdbool.h>
#include "ssd1306.h"
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"

#define DISPLAY_DEFAULT_FONT Font_6x8
//#define ENCODER1_POSITION 14
//#define ENCODER2_POSITION 4
#define RIGHT_ENCODER_POSITION 12

void display_init(void);
void display_splash_screen(void);
void display_clear_screen(SSD1306_COLOR color);
void display_start_screen(void);
int16_t display_string(const char *str, uint8_t line_number, uint8_t cursor_position, SSD1306_COLOR color, bool ceol_flag);
int16_t display_string_to_status_line(const char *str, uint8_t position, SSD1306_COLOR color, bool ceol_flag);
void display_clear_page(SSD1306_COLOR color);

#endif /* INC_DISPLAY_H_ */
