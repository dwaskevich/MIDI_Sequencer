/*
 * tasks.c
 *
 *  Created on: Jan 20, 2026
 *      Author: dwask
 */

#include "stdio.h"
#include "tasks.h"
#include "scheduler.h"
#include "main.h"  // For GPIO or other shared symbols
#include "display.h"

extern volatile bool is_sequencer_on;
extern volatile uint8_t channel_range;

// Task implementation

void heartbeat(void)
{
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

void read_encoders(void)
{
	char printBuffer[30];
	static int16_t encoder1_previous_value = 0, encoder2_previous_value = 0;

	int16_t encoder1_current_value = (int16_t)__HAL_TIM_GET_COUNTER(&htim2) / 2;
	int16_t encoder2_current_value = (int16_t)__HAL_TIM_GET_COUNTER(&htim3) / 4;

	int16_t delta1 = (int16_t)(encoder1_current_value - encoder1_previous_value);
	if (0 != delta1)
	{
		channel_range = (uint8_t)encoder1_current_value % 8;
		if(0 == channel_range)
			channel_range = 1;
//		sprintf(printBuffer, "%-3d", encoder1_current_value);
		sprintf(printBuffer, "%-3d", channel_range);
		display_string_to_status_line(printBuffer, ENCODER1_POSITION);
		encoder1_previous_value = encoder1_current_value;
	}

	int16_t delta2 = (int16_t)(encoder2_current_value - encoder2_previous_value);
	if (0 != delta2)
	{
		is_sequencer_on = encoder2_current_value % 2;
//		sprintf(printBuffer, "%-3d", encoder2_current_value);
		sprintf(printBuffer, is_sequencer_on ? "On " : "Off");
		display_string_to_status_line(printBuffer, ENCODER2_POSITION);
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


// --- Task initialization/registration ---
void tasks_init(void) {
    scheduler_add_task(heartbeat, 500);
    scheduler_add_task(read_encoders, 21);
//    scheduler_add_task(play_sequence, 500);
//    scheduler_add_task(poll_buttons, 13);
    // Add more tasks here
}

