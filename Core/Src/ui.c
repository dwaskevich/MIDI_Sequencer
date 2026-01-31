/*
 * ui.c
 *
 *  Created on: Jan 27, 2026
 *      Author: dwask
 */

#include <stdio.h>
#include "main.h"
#include "ui.h"
#include "display.h"

ui_state_t ui_state;

//const char *menuNames[] = {"On/Off", "Scale", "Key", "Tempo", "Octave Range", "Channel" };

#define X(name, str) str,
const char *menuNames[] = {
#include "ui_menu_def.h"   // <‑‑ THIS is the include
};
#undef X


struct uiEncoderValues ui_encoderValues = {0};
struct uiSettings ui_settings = {0, MAJOR, 0, 200, 167, 0, 1};

const size_t menuCount = sizeof(menuNames) / sizeof(menuNames[0]);
int16_t menuIndex = 0;
extern volatile bool is_sequencer_on;
extern volatile uint8_t channel_range;
extern volatile uint16_t tempo;

void handle_menu_encoder(int16_t encoder_value, int16_t delta)
{
	char printBuffer[80];
    menuIndex += delta;

    if (menuIndex < 0)
    	menuIndex = 0;
    else if (menuIndex >= menuCount)
        menuIndex = menuCount - 1;

    display_string_to_status_line(menuNames[menuIndex], 0, White, false);

    switch (menuIndex) {
		case MENU_ON_OFF:
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.on_off); /* restore selection encoder to previous counter value (prevents jumping) */
//			sprintf(printBuffer, ui_settings.on_off ? "On   " : "Off  "); /* post status to display */
			sprintf(printBuffer, "%s %d", (ui_settings.on_off ? "On " : "Off "), ui_encoderValues.on_off); /* post status to display */
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);
			break;

		case MENU_SCALE:
			ui_encoderValues.menu_encoder_previous_value = ui_encoderValues.scale;
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.scale); /* restore selection encoder to previous counter value (prevents jumping) */
//			sprintf(printBuffer, ui_settings.scale ? "Minor" : "Major");
			sprintf(printBuffer, "%s %d", (ui_settings.scale ? "Minor" : "Major"), ui_encoderValues.scale);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);
			break;

		case MENU_KEY:
			sprintf(printBuffer, "%d      ", 0);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);
			break;

		case MENU_TEMPO:
//			sprintf(printBuffer, "%d/%d", tempo, tempo + 500);
			sprintf(printBuffer, "%d/%d", tempo, (uint16_t)(tempo*2.67));
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);
			break;

		case MENU_HUMANIZE:
			sprintf(printBuffer, "%d      ", 0);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);
			break;

		case MENU_OCTAVE_RANGE:
			sprintf(printBuffer, "%d      ", 0);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);
			break;

		case MENU_CHANNEL:
			sprintf(printBuffer, "%-3d     ", channel_range);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);

			break;

		default:
			break;
	}
}

void handle_value_encoder(int16_t encoder_value, int16_t delta)
{
	char printBuffer[80];
	switch (menuIndex) {
	    case MENU_ON_OFF:
	    	is_sequencer_on = encoder_value % 2; /* use encoder value to set sequencer on or off */
	    	ui_settings.on_off = is_sequencer_on; /* update ui settings for this menu item */
	    	ui_encoderValues.on_off = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
//	    	sprintf(printBuffer, ui_settings.on_off ? "On   " : "Off  "); /* post status to display */
//	    	sprintf(printBuffer, "%s %d", (ui_settings.on_off ? "On " : "Off "), ui_encoderValues.on_off);
	    	sprintf(printBuffer, "%s %d %d", (ui_settings.on_off ? "On " : "Off "), ui_encoderValues.on_off, is_sequencer_on);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);
	        break;

	    case MENU_SCALE:
	    	ui_encoderValues.scale = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
			ui_settings.scale = encoder_value % 2 ? MAJOR : MINOR;
	    	sprintf(printBuffer, ui_settings.scale ? "Minor" : "Major");
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);
			break;

	    case MENU_KEY:
	    	ui_encoderValues.key = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
			sprintf(printBuffer, "%d      ", encoder_value);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);
			break;

	    case MENU_TEMPO:
	    	ui_encoderValues.tempo = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
	    	tempo += delta * 100;
	    	if(tempo < 100)
	    	{
	    		tempo = 100;
	    	}
	    	if(tempo > 1500)
	    	{
	    		tempo = 1500;
	    	}
//			sprintf(printBuffer, "%d/%d", tempo, tempo + 500);
			sprintf(printBuffer, "%d/%d", tempo, (uint16_t)(tempo*2.67));
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);
			break;

	    case MENU_HUMANIZE:
	    	ui_encoderValues.humanize = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
	    	sprintf(printBuffer, "%d      ", encoder_value);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);
			break;

	    case MENU_OCTAVE_RANGE:
	    	ui_encoderValues.octave_range = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
			sprintf(printBuffer, "%d      ", encoder_value);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);
			break;

	    case MENU_CHANNEL:
	    	ui_encoderValues.channel = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
			channel_range = (uint8_t)encoder_value % 8;
			if(0 == channel_range)
				channel_range = 1;
			sprintf(printBuffer, "%-3d     ", channel_range);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);

	        break;

	    default:
	    	break;}

}



