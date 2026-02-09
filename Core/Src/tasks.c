/*
 * tasks.c
 *
 *  Created on: Jan 20, 2026
 *      Author: dwask
 */

#include <stdio.h>
#include "tasks.h"
#include "scheduler.h"
#include "main.h" /* needed for GPIO or other shared symbols */
#include "buttons.h"
#include "display.h"
#include "ui.h"

char printBuffer[30];

/* task implementations */

void heartbeat(void)
{
	static uint16_t heartbeat_counter = 0;

	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

	if(true == ui_heartbeat_display_update_flag) /* ui visual indication of which of two parameters will be adjusted by encoder */
	{
		if(heartbeat_counter % 2)
			display_string_to_status_line(ui_display_buffer_a, RIGHT_ENCODER_POSITION, White, true); /* blink buffer a */
		else
			display_string_to_status_line(ui_display_buffer_b, RIGHT_ENCODER_POSITION, White, true); /* blink buffer b */
	}
	heartbeat_counter++;
}

void read_encoders(void)
{
	int16_t value_encoder_current_value = (int16_t)__HAL_TIM_GET_COUNTER(&htim2) / 2;
	int16_t menu_encoder_current_value = (int16_t)__HAL_TIM_GET_COUNTER(&htim3) / 4;

	if (true == value_encoder_ignore_next) /* cosmetics ... will prevent a menu "jump" when moving between menus */
	{
		ui_encoderValues.value_encoder_previous_value = value_encoder_current_value;
	    value_encoder_ignore_next = false;
	    return;   /* skip delta processing once ... prevents unwanted "false detection" of value_encoder movement */
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
		ui_heartbeat_display_update_flag = false;
		ui_encoderValues.menu_encoder_previous_value = menu_encoder_current_value;
		handle_menu_encoder(menu_encoder_current_value, delta_menu_encoder);
	}
}

static void poll_buttons(void) {
    button_poll();

    switch (button_get_event(BUTTON_1)) {
        case BUTTON_EVENT_SHORT_PRESS:
        	ui_primary_secondary_value_flag = !ui_primary_secondary_value_flag;
        	switch (menuIndex) {
        		case MENU_OCTAVE_RANGE:
        			handle_value_encoder(0, 0);
        			break;

        		case MENU_CHANNEL:
        			if(false == ui_primary_secondary_value_flag)
        			{
        				handle_value_encoder(ui_encoderValues.channel_low, 0);
        			}
        			else
        			{
        				handle_value_encoder(ui_encoderValues.channel_high, 0);
        			}
        			break;
        	}
            break;
        case BUTTON_EVENT_LONG_PRESS:

            break;
        default:
            break;
    }

    switch (button_get_event(BUTTON_2)) {
        case BUTTON_EVENT_SHORT_PRESS:
        	ui_heartbeat_display_update_flag = false;
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
    scheduler_add_task(heartbeat, 200);
    scheduler_add_task(read_encoders, 11);
    scheduler_add_task(poll_buttons, 13);
    // Add more tasks here
}

