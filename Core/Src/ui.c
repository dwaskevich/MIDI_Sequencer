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
#include "notes.h"
#include "modes.h"
#include "presets.h"
#include "notes.h"

#define X(name, str) str,
const char *menuNames[] = {
#include "ui_menu_def.h"   // <‑‑ THIS is the include
};
#undef X

#define MIDI_OCTAVE_LOW  0
#define MIDI_OCTAVE_HIGH  8
#define MIDI_MAX_CHANNELS 16

struct uiEncoderValues ui_encoderValues = {0};
bool value_encoder_ignore_next = false;
struct uiSettings ui_settings = {OFF, OFF, MODE_IONIAN, NOTE_C, 600, 167, 2, 6, 1, 1, PRESET_PEACEFUL};
bool ui_heartbeat_display_update_flag = false;
bool ui_primary_secondary_value_flag = false;

char ui_display_buffer_a[16];
char ui_display_buffer_b[16];

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
			display_string_to_status_line(ui_settings.on_off ? "On" : "Off", RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

		case MENU_CHORDS:
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.chords); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.chords; /* save/record previous value for use in tasks.c delta calculation */
			display_string_to_status_line(ui_settings.chords ? "Triad" : "Off", RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

		case MENU_MODE:
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.mode); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.mode; /* save/record previous value for use in tasks.c delta calculation */
			display_string_to_status_line(mode_display_names[ui_settings.mode], RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

		case MENU_KEY:
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.key); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.key; /* save/record previous value for use in tasks.c delta calculation */
			display_string_to_status_line(note_to_string(ui_settings.key), RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

		case MENU_TEMPO:
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.tempo); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.tempo; /* save/record previous value for use in tasks.c delta calculation */
			sprintf(printBuffer, "%d/%d", ui_settings.tempo_bpm / 2, (uint16_t)((ui_settings.tempo_bpm / 2) * 2.67));
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

		case MENU_HUMANIZE:
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.humanize); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.humanize; /* save/record previous value for use in tasks.c delta calculation */
			sprintf(printBuffer, "%d", ui_settings.humanize);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

		case MENU_OCTAVE_RANGE:
			ui_primary_secondary_value_flag = false; /* this menu has multi-value selection, clear primary/secondary flag */
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.octave_range_low); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.octave_range_low; /* save/record previous value for use in tasks.c delta calculation */
			sprintf(ui_display_buffer_a, "%d / %d", ui_settings.octave_low, ui_settings.octave_high);
			sprintf(ui_display_buffer_b, "  / %d", ui_settings.octave_high);
			display_string_to_status_line(ui_display_buffer_a, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

		case MENU_CHANNEL:
			ui_primary_secondary_value_flag = false; /* this menu has multi-value selection, clear primary/secondary flag */
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.channel_low); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.channel_low; /* save/record previous value for use in tasks.c delta calculation */
			sprintf(ui_display_buffer_a, "%d / %d", ui_settings.channel_low, ui_settings.channel_high);
			sprintf(ui_display_buffer_b, "  / %d", ui_settings.channel_high);
			display_string_to_status_line(ui_display_buffer_a, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

		case MENU_PRESETS:
			__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.presets); /* restore value selection encoder to previous counter value (prevents jumping) */
			ui_encoderValues.value_encoder_previous_value = ui_encoderValues.presets; /* save/record previous value for use in tasks.c delta calculation */
			display_string_to_status_line(presets[ui_settings.presets].name, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
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
	    	sprintf(printBuffer, "%s", (ui_settings.on_off ? "On" : "Off"));
	    	display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
	        break;

	    case MENU_CHORDS:
			ui_settings.chords = encoder_value % 2; /* use encoder value to turn chord generation on or off */
			ui_encoderValues.chords = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
			sprintf(printBuffer, "%s", (ui_settings.chords ? "Triad" : "Off"));
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

	    case MENU_MODE:
	    	int16_t new_mode = ui_settings.mode + delta;
	    	if(new_mode < 0)
	    		new_mode = 0;
	    	else if(new_mode >= MODE_COUNT)
	    		new_mode = MODE_COUNT - 1;
	    	ui_settings.mode = new_mode; /* update ui settings for this menu item */
	    	ui_encoderValues.mode = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
	    	display_string_to_status_line(mode_display_names[ui_settings.mode], RIGHT_ENCODER_POSITION, White, true); /* post status to display */
	    	/* build new note selection list */
	    	scale_length = build_scale(ui_settings.key, mode_intervals[ui_settings.mode], mode_interval_count[ui_settings.mode], ui_settings.octave_low, ui_settings.octave_high, scale_notes, sizeof(scale_notes)/sizeof(scale_notes[0]));
			break;

	    case MENU_KEY:
	    	ui_settings.key += delta; /* update ui settings for this menu item */
	    	ui_settings.key = ui_settings.key % 12;
	    	ui_encoderValues.key = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
	    	sprintf(printBuffer, "%s / %d", note_to_string(ui_settings.key), ui_settings.key);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);
			/* build new note selection list */
			scale_length = build_scale(ui_settings.key, mode_intervals[ui_settings.mode], mode_interval_count[ui_settings.mode], ui_settings.octave_low, ui_settings.octave_high, scale_notes, sizeof(scale_notes)/sizeof(scale_notes[0]));
			break;

	    case MENU_TEMPO:
	    	ui_settings.tempo_bpm -= delta * 200;
	    	if(ui_settings.tempo_bpm < 200)
	    	{
	    		ui_settings.tempo_bpm = 200;
	    	}
	    	if(ui_settings.tempo_bpm > 3000)
	    	{
	    		ui_settings.tempo_bpm = 3000;
	    	}
	    	ui_settings.tempo_bpm = ui_settings.tempo_bpm; /* update ui settings for this menu item */
	    	ui_encoderValues.tempo = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
			sprintf(printBuffer, "%d/%d", ui_settings.tempo_bpm / 2, (uint16_t)((ui_settings.tempo_bpm / 2) * 2.67));
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

	    case MENU_HUMANIZE:
	    	ui_settings.humanize = encoder_value; /* update ui settings for this menu item */
	    	ui_encoderValues.humanize = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
	    	sprintf(printBuffer, "%d", ui_settings.humanize);
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true); /* post status to display */
			break;

	    case MENU_OCTAVE_RANGE:
	    	ui_heartbeat_display_update_flag = true; /* notify heartbeat task to "blink" selected value */
	    	if(false == ui_primary_secondary_value_flag) /* modify/operate on primary (low) value */
	    	{
	    		ui_settings.octave_low += delta; /* update ui settings for this menu item */
	    		if(ui_settings.octave_low <= MIDI_OCTAVE_LOW)
	    		{
	    			ui_settings.octave_low = MIDI_OCTAVE_LOW;
	    		}
	    		if(ui_settings.octave_low >= ui_settings.octave_high - 1)
	    		{
	    			ui_settings.octave_low = ui_settings.octave_high - 1;
	    		}
				ui_encoderValues.octave_range_low = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
				sprintf(ui_display_buffer_a, "%d / %d", ui_settings.octave_low, ui_settings.octave_high);
				sprintf(ui_display_buffer_b, "  / %d", ui_settings.octave_high);
	    	}
	    	else /* modify/operate on secondary (high) value */
	    	{
	    		ui_settings.octave_high += delta; /* update ui settings for this menu item */
	    		if(ui_settings.octave_high >= MIDI_OCTAVE_HIGH)
	    		{
					ui_settings.octave_high = MIDI_OCTAVE_HIGH;
	    		}
				if(ui_settings.octave_high <= ui_settings.octave_low + 1)
				{
					ui_settings.octave_high = ui_settings.octave_low + 1;
				}
				ui_encoderValues.octave_range_high = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
				sprintf(ui_display_buffer_a, "%d / %d", ui_settings.octave_low, ui_settings.octave_high);
				sprintf(ui_display_buffer_b, "%d /", ui_settings.octave_low);
	    	}
	    	/* build new note selection list */
	    	scale_length = build_scale(ui_settings.key, mode_intervals[ui_settings.mode], mode_interval_count[ui_settings.mode], ui_settings.octave_low, ui_settings.octave_high, scale_notes, sizeof(scale_notes)/sizeof(scale_notes[0]));
			break;

	    case MENU_CHANNEL:
	    	ui_heartbeat_display_update_flag = true; /* notify heartbeat task to "blink" selected value */
	    	if(false == ui_primary_secondary_value_flag) /* modify/operate on primary (low) value */
			{
	    		ui_settings.channel_low += delta; /* update ui settings for this menu item */
				if(ui_settings.channel_low <= 0)
				{
					ui_settings.channel_low = 1;
				}
				if(ui_settings.channel_low >= MIDI_MAX_CHANNELS)
				{
					ui_settings.channel_low = MIDI_MAX_CHANNELS;
				}
				if(ui_settings.channel_low >= ui_settings.channel_high)
				{
					ui_settings.channel_high = ui_settings.channel_low; /* track channel high with channel low */
				}
				ui_encoderValues.channel_low = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
				sprintf(ui_display_buffer_a, "%d / %d", ui_settings.channel_low, ui_settings.channel_high);
				sprintf(ui_display_buffer_b, "  / %d", ui_settings.channel_high);
			}
			else /* modify/operate on secondary (high) value */
			{
				ui_settings.channel_high += delta; /* update ui settings for this menu item */
				if(ui_settings.channel_high <= 0)
				{
					ui_settings.channel_high = 1;
				}
				if(ui_settings.channel_high <= ui_settings.channel_low)
				{
					ui_settings.channel_low = ui_settings.channel_high;
				}
				if(ui_settings.channel_high >= MIDI_MAX_CHANNELS)
				{
					ui_settings.channel_high = MIDI_MAX_CHANNELS;
				}
				ui_encoderValues.channel_high = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
				sprintf(ui_display_buffer_a, "%d / %d", ui_settings.channel_low, ui_settings.channel_high);
				sprintf(ui_display_buffer_b, "%d /", ui_settings.channel_low);
			}
	        break;

	    case MENU_PRESETS:
			ui_settings.presets += delta;
			if(ui_settings.presets >= PRESET_COUNT)
				ui_settings.presets = PRESET_COUNT - 1;
			else if(ui_settings.presets <= 0)
				ui_settings.presets = 0;
			ui_encoderValues.presets = __HAL_TIM_GET_COUNTER(&htim2); /* store/remember counter value for next entry into this menu by left encoder */
			display_string_to_status_line(presets[ui_settings.presets].name, RIGHT_ENCODER_POSITION, White, true); /* post preset name to display */
			/* apply preset settings to ui_settings */
			ui_settings.key         = presets[ui_settings.presets].key;
			ui_settings.mode        = presets[ui_settings.presets].mode;
			ui_settings.octave_low  = presets[ui_settings.presets].octave_low;
			ui_settings.octave_high = presets[ui_settings.presets].octave_high;
			printf("[preset] %s - ", presets[ui_settings.presets].name);
			scale_length = build_scale(ui_settings.key, mode_intervals[ui_settings.mode], mode_interval_count[ui_settings.mode], ui_settings.octave_low, ui_settings.octave_high, scale_notes, sizeof(scale_notes)/sizeof(scale_notes[0]));
			break;

	    default:
	    	break;}

}



