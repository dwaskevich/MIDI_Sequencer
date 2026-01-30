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

extern volatile bool is_sequencer_on;
//extern volatile uint8_t channel_range;

char printBuffer[30];

// Task implementation

void heartbeat(void)
{
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

void read_encoders(void)
{
	static int16_t encoder1_previous_value = 0, encoder2_previous_value = 0;

	int16_t encoder1_current_value = (int16_t)__HAL_TIM_GET_COUNTER(&htim2) / 2;
	int16_t encoder2_current_value = (int16_t)__HAL_TIM_GET_COUNTER(&htim3) / 4;

	int16_t delta1 = (int16_t)(encoder1_current_value - encoder1_previous_value);
	if (0 != delta1)
	{
//		channel_range = (uint8_t)encoder1_current_value % 8;
//		if(0 == channel_range)
//			channel_range = 1;
//		sprintf(printBuffer, "%-3d", channel_range);
//		display_string_to_status_line(printBuffer, ENCODER1_POSITION);
		handle_right_encoder(encoder1_current_value, delta1);
		encoder1_previous_value = encoder1_current_value;
	}

	int16_t delta2 = (int16_t)(encoder2_current_value - encoder2_previous_value);
	if (0 != delta2)
	{
//		display_string_to_status_line(menuNames[encoder2_previous_value + delta2], ENCODER1_POSITION);
		handle_left_encoder(encoder2_current_value, delta2);
		encoder2_previous_value = encoder2_current_value;
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
        	if(is_sequencer_on)
        		is_sequencer_on = false;
        	else
        		is_sequencer_on = true;
        	menuIndex = MENU_ON_OFF;
        	ui_encoderValues.on_off = is_sequencer_on; /* ensures ui menu controls are in sync with interrupt-driven change */
        	ui_settings.on_off = is_sequencer_on;
        	__HAL_TIM_SET_COUNTER(&htim2, ui_encoderValues.on_off);
        	display_string_to_status_line(menuNames[MENU_ON_OFF], 0, White, false);
        	sprintf(printBuffer, ui_settings.on_off ? "On " : "Off");
			display_string_to_status_line(printBuffer, RIGHT_ENCODER_POSITION, White, true);

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
    scheduler_add_task(read_encoders, 5);
//    scheduler_add_task(play_sequence, 500);
    scheduler_add_task(poll_buttons, 13);
    // Add more tasks here
}

