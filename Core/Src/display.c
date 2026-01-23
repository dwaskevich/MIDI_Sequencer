/*
 * display.c
 *
 *  Created on: Jan 20, 2026
 *      Author: dwask
 */

#include "stdio.h"
#include "string.h"
#include "display.h"


#define STATUS_LINE_LINE_NUMBER  0
#define STATUS_LINE_STATUS_WIDTH  21

char hello_world_str[] = "   MIDI Sequencer";
char print_buffer[128];
uint8_t line_number = 0;

static uint8_t line_height;
static uint8_t chars_per_line;
static uint8_t number_lines;

void display_init(void)
{
  ssd1306_Init();
  line_height = DISPLAY_DEFAULT_FONT.height + 1;
  chars_per_line = SSD1306_WIDTH / DISPLAY_DEFAULT_FONT.width;
  number_lines = (SSD1306_HEIGHT / DISPLAY_DEFAULT_FONT.height) - 1;
}

void display_splash_screen()
{
  ssd1306_FillRectangle(0, 0, SSD1306_WIDTH, 14, White);
  ssd1306_FillRectangle(2, 3, 125, 11, Black);
  ssd1306_SetCursor(4, 4);
  ssd1306_WriteString(hello_world_str, DISPLAY_DEFAULT_FONT, White);
  line_number = DISPLAY_DEFAULT_FONT.height * 2;
  ssd1306_SetCursor(3, 20);
  ssd1306_WriteString("SSD1306 OLED 128x64", DISPLAY_DEFAULT_FONT, White);
  sprintf(print_buffer, "Font -> H=%d, W=%d", DISPLAY_DEFAULT_FONT.height, DISPLAY_DEFAULT_FONT.width);
  line_number = DISPLAY_DEFAULT_FONT.height * 3 + 1;
  ssd1306_SetCursor(3, 29);
  ssd1306_WriteString(print_buffer, DISPLAY_DEFAULT_FONT, White);
  sprintf(print_buffer, "Characters/line = %d", chars_per_line);
  line_number = DISPLAY_DEFAULT_FONT.height * 4 + 2;
  ssd1306_SetCursor(3, 38);
  ssd1306_WriteString(print_buffer, DISPLAY_DEFAULT_FONT, White);
  sprintf(print_buffer, "Number of lines = %d", number_lines);
  line_number = DISPLAY_DEFAULT_FONT.height * 5 + 3;
  ssd1306_SetCursor(3, 47);
  ssd1306_WriteString(print_buffer, DISPLAY_DEFAULT_FONT, White);

  ssd1306_UpdateScreen();
}

void display_clear_screen(SSD1306_COLOR color)
{
	ssd1306_Fill(color);
	ssd1306_UpdateScreen();
}

void display_start_screen(void)
{
  display_clear_screen(Black);
  display_string_to_status_line("Tempo 0    Range 0", 0);
}

int16_t display_string(char *str, uint8_t line_number, uint8_t cursor_position, SSD1306_COLOR color, bool ceol_flag)
{
	uint8_t x_position = cursor_position * DISPLAY_DEFAULT_FONT.width;
	uint8_t cursor_end_position = cursor_position + strlen(str);
	if(str == NULL || cursor_end_position > chars_per_line)
		return -1;

	if(line_number > number_lines - 1)
		line_number = number_lines - 1;
	ssd1306_SetCursor(x_position, line_number * line_height);
	ssd1306_WriteString(str, DISPLAY_DEFAULT_FONT, color);
	if(true == ceol_flag)
	{
		while(cursor_end_position++ < chars_per_line)
			ssd1306_WriteChar(' ', DISPLAY_DEFAULT_FONT, color);
	}
	ssd1306_UpdateScreen();
	return 0;
}

int16_t display_string_to_status_line(char *str, uint8_t position)
{
	display_string(str, STATUS_LINE_LINE_NUMBER, position, White, false);

	return 0;
}

void display_clear_page(SSD1306_COLOR color)
{
	ssd1306_FillRectangle(0, line_height, SSD1306_WIDTH, SSD1306_HEIGHT, Black);
}


