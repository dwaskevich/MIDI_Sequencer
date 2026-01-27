/*
 * buttons.c
 *
 *  Created on: Jan 26, 2026
 *      Author: dwask
 */

#include "buttons.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"

#define LONG_PRESS_THRESHOLD_MS 500
#define DEBOUNCE_GUARD_MS 20

typedef struct {
    volatile bool pressed;
    volatile bool released;
    uint32_t press_time;
    uint32_t release_time;
    uint32_t last_event_time;
    ButtonEvent pending_event;
} ButtonState;

static ButtonState button_states[BUTTON_COUNT];

void button_init(void) {
    for (int i = 0; i < BUTTON_COUNT; ++i) {
        button_states[i].pressed = false;
        button_states[i].released = false;
        button_states[i].pending_event = BUTTON_EVENT_NONE;
    }
}

void button_exti_trigger(ButtonID id, bool is_pressed) {
    uint32_t now = HAL_GetTick();

    // Debounce: ignore if too soon after last edge
    if ((now - button_states[id].last_event_time) < DEBOUNCE_GUARD_MS)  {
		return;
	}
    button_states[id].last_event_time = now; /* mark current event time */

    if(button_states[id].pressed && is_pressed)
	{
		/* shouldn't happen ... missed release edge/interrupt/event, update press_time to recover */
		button_states[id].press_time = now;
	}
    else if (!button_states[id].pressed && is_pressed) {
		/* button just pressed */
        button_states[id].pressed = true;
        button_states[id].press_time = now;
    } else {
    	// button just released ... always accept release if button was previously pressed
		if(button_states[id].pressed) {
			button_states[id].released = true;
			button_states[id].release_time = now;
		}
    }
}

/* button event detection here */
void button_poll(void) {
    for (int i = 0; i < BUTTON_COUNT; ++i) {
    	bool released = false;
		uint32_t press_time = 0, release_time = 0;

		__disable_irq(); // Begin critical section
		if (button_states[i].released) {
			released = true;
			button_states[i].released = false;
			button_states[i].pressed = false;
			press_time = button_states[i].press_time;
			release_time = button_states[i].release_time;
		}
		__enable_irq(); // End critical section

		if (released) {
			uint32_t duration = release_time - press_time;
            if (duration >= LONG_PRESS_THRESHOLD_MS) {
                button_states[i].pending_event = BUTTON_EVENT_LONG_PRESS;
                printf("Button %d: LONG press (%lu ms)\r\n", i + 1, duration);
                switch(i)
                {
                	case BUTTON_1: /* long press */


                		break;

                	case BUTTON_2: /* long press */


						break;

                	default:
                		break;

                }
            } else {
                button_states[i].pending_event = BUTTON_EVENT_SHORT_PRESS;
                printf("Button %d: SHORT press (%lu ms)\r\n", i + 1, duration);
                switch(i)
				{
					case BUTTON_1: /* short press */


						break;

					case BUTTON_2: /* short press */


						break;

					default:
						break;

				}
            }
        }
    }
}

ButtonEvent button_get_event(ButtonID id) {
    ButtonEvent event = button_states[id].pending_event;
    button_states[id].pending_event = BUTTON_EVENT_NONE;
    return event;
}

