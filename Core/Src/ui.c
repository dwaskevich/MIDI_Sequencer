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

#define X(name, str) str,
const char *menuNames[] = {
#include "ui_menu_def.h"   // <‑‑ THIS is the include
};
#undef X

struct uiEncoderValues ui_encoderValues = {0};
bool value_encoder_ignore_next = false;
struct uiSettings ui_settings = {0, MAJOR, 0, 300, 167, 0, 1};

const size_t menuCount = sizeof(menuNames) / sizeof(menuNames[0]);
int16_t menuIndex = 0;

void handle_menu_encoder(int16_t encoder_value, int16_t delta)
{
	char printBuffer[80];
    menuIndex += delta;

    if (menuIndex < 0)
    	menuIndex = 0;
    else if (menuIndex >= menuCount)
        menuIndex = menuCount - 1;

    display_string_to_status_line(menuNames[menuIndex], 0, White, false);

    value_encoder_ignore_next = true;

    switch (menuIndex) {
		case MENU_ON_OFF:
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.on_off); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.on_off; /* save/record previous value for use in tasks.c delta calculation */
			display_string_to_status_line(ui_settings.on_off ? "xOn" : "xOff", RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

		case MENU_SCALE:
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.scale); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.scale; /* save/record previous value for use in tasks.c delta calculation */
			display_string_to_status_line(ui_settings.scale ? "xMinor" : "xMajor", RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

		case MENU_KEY:
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.key); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.key; /* save/record previous value for use in tasks.c delta calculation */
			sprintf(printBuffer, "x%d", ui_settings.key);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

		case MENU_TEMPO:
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.tempo); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.tempo; /* save/record previous value for use in tasks.c delta calculation */
			sprintf(printBuffer, "x%d/%d", ui_settings.tempo_bpm, (uint16_t)(ui_settings.tempo_bpm*2.67));
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

		case MENU_HUMANIZE:
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.humanize); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.humanize; /* save/record previous value for use in tasks.c delta calculation */
			sprintf(printBuffer, "x%d", ui_settings.humanize);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

		case MENU_OCTAVE_RANGE:
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.octave_range); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.octave_range; /* save/record previous value for use in tasks.c delta calculation */
			sprintf(printBuffer, "x%d", ui_settings.octave_range);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

		case MENU_CHANNEL:
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.channel); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.channel; /* save/record previous value for use in tasks.c delta calculation */
			sprintf(printBuffer, "x%d", ui_settings.channel);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */

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
	    	ui_settings.on_off = encoder_value % 2; /* use encoder value to set sequencer on or off */
	    	ui_encoderValues.on_off = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
	    	sprintf(printBuffer, "%s %d", (ui_settings.on_off ? "On" : "Off"), ui_encoderValues.on_off);
	    	display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
	        break;

	    case MENU_SCALE:
	    	ui_settings.scale = encoder_value % 2 ? MAJOR : MINOR; /* update ui settings for this menu item */
	    	ui_encoderValues.scale = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
	    	sprintf(printBuffer, ui_settings.scale ? "Minor" : "Major");
	    	display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

	    case MENU_KEY:
	    	ui_settings.key = encoder_value; /* update ui settings for this menu item */
	    	ui_encoderValues.key = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
	    	sprintf(printBuffer, "%d", ui_settings.key);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

	    case MENU_TEMPO:
	    	ui_settings.tempo_bpm += delta * 100;
	    	if(ui_settings.tempo_bpm < 100)
	    	{
	    		ui_settings.tempo_bpm = 100;
	    	}
	    	if(ui_settings.tempo_bpm > 1500)
	    	{
	    		ui_settings.tempo_bpm = 1500;
	    	}
	    	ui_settings.tempo_bpm = ui_settings.tempo_bpm; /* update ui settings for this menu item */
	    	ui_encoderValues.tempo = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
			sprintf(printBuffer, "%d/%d", ui_settings.tempo_bpm, (uint16_t)(ui_settings.tempo_bpm*2.67));
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

	    case MENU_HUMANIZE:
	    	ui_settings.humanize = encoder_value; /* update ui settings for this menu item */
	    	ui_encoderValues.humanize = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
	    	sprintf(printBuffer, "%d", ui_settings.humanize);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

	    case MENU_OCTAVE_RANGE:
	    	ui_settings.octave_range = encoder_value; /* update ui settings for this menu item */
	    	ui_encoderValues.octave_range = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
	    	sprintf(printBuffer, "%d", ui_settings.octave_range);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

	    case MENU_CHANNEL:
			ui_settings.channel = (uint8_t)encoder_value % 8;
			if(0 == ui_settings.channel)
				ui_settings.channel = 1;
			ui_settings.channel = ui_settings.channel; /* update ui settings for this menu item */
			ui_encoderValues.channel = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
			sprintf(printBuffer, "%d", ui_settings.channel);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */

	        break;

	    default:
	    	break;}

}



