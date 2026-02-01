/*
 * tasks.c
 *
 *  Created on: Jan 20, 2026
 *      Author: dwask
 */

#include <stdio.h>
#include "tasks.h"
#include "scheduler.h"
#include "main.h"  // For GPIO or other shared symbols
#include "buttons.h"
#include "display.h"
#include "ui.h"

char printBuffer[30];

// Task implementation

void heartbeat(void)
{
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

void read_encoders(void)
{
	int16_t value_encoder_current_value = (int16_t)__HAL_TIM_GET_COUNTER(&htim2) / 2;
	int16_t menu_encoder_current_value = (int16_t)__HAL_TIM_GET_COUNTER(&htim3) / 4;

	if (true == value_encoder_ignore_next)
	{
		ui_encoderValues.value_encoder_previous_value = value_encoder_current_value;
	    value_encoder_ignore_next = false;
	    return;   // skip delta processing once
	}

	int16_t delta_value_encoder = (int16_t)(value_encoder_current_value - ui_encoderValues.value_encoder_previous_value);
	if (0 != delta_value_encoder)
	{
		ui_encoderValues.value_encoder_previous_value = value_encoder_current_value;
		handle_value_encoder(value_encoder_current_value, delta_value_encoder);
	}

	int16_t delta_menu_encoder = (int16_t)(menu_encoder_current_value - ui_encoderValues.menu_encoder_previous_value);
	if (0 != delta_menu_encoder)
	{
		ui_encoderValues.menu_encoder_previous_value = menu_encoder_current_value;
		handle_menu_encoder(menu_encoder_current_value, delta_menu_encoder);
	}
}

//void play_sequence(void)
//{
//	uint8_t previous_note = 0;
//	uint8_t note;
//	while(previous_note == (note = c_major_scale[randomize(0, sizeof(c_major_scale)/sizeof(c_major_scale[0]) - 1)]))
//		;
//	previous_note = note;
//	midiSendNoteOn(note, 4, 80);
//}

static void poll_buttons(void) {
    button_poll();

    switch (button_get_event(BUTTON_1)) {
        case BUTTON_EVENT_SHORT_PRESS:

            break;
        case BUTTON_EVENT_LONG_PRESS:

            break;
        default:
            break;
    }

    switch (button_get_event(BUTTON_2)) {
        case BUTTON_EVENT_SHORT_PRESS:
        	/* toggle sequencer on/off */
        	if(ui_settings.on_off)
        		ui_settings.on_off = 0;
        	else
        		ui_settings.on_off = 1;
        	menuIndex = MENU_ON_OFF;
        	__HAL_TIM_SET_COUNTER(&htim2, ui_settings.on_off * 2);  /* ensures ui menu controls are in sync with interrupt-driven change */
        	ui_encoderValues.value_encoder_previous_value = ui_settings.on_off * 2; /* save/record previous value for use in tasks.c delta calculation */
        	display_string_to_status_line(menuNames[MENU_ON_OFF], 0, White, false);
			display_string_to_status_line(ui_settings.on_off ? "On" : "Off", RIGHT_ENCODER_POSITION, White, true);

            break;
        case BUTTON_EVENT_LONG_PRESS:

            break;
        default:
            break;
    }
}


// --- Task initialization/registration ---
void tasks_init(void) {
    scheduler_add_task(heartbeat, 500);
    scheduler_add_task(read_encoders, 11);
//    scheduler_add_task(play_sequence, 500);
    scheduler_add_task(poll_buttons, 13);
    // Add more tasks here
}

