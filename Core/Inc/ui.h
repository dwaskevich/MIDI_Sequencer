/*
 * ui.h
 *
 *  Created on: Jan 27, 2026
 *      Author: dwask
 */

#ifndef INC_UI_H_
#define INC_UI_H_

#include "stdbool.h"
#include "stdint.h"
#include <stddef.h>

typedef enum {
	UI_STATE_SEQUENCER_ACTIVE,
	UI_STATE_MENU_SELECT,
	UI_STATE_CONFIGURE_ITEM,
	UI_STATE_CONFIRM_SETTING,
	UI_STATE_HANDLE_ERROR
} ui_state_t;

typedef enum {
#define X(name, str) MENU_##name,
#include "ui_menu_def.h"
#undef X
    MENU_COUNT
} menu_id_t;

typedef enum {
	MAJOR,
	MINOR
} eScale_t;

struct uiEncoderValues {
	int16_t  menu_encoder_previous_value;
	int16_t  value_encoder_previous_value;
	int16_t  on_off;
	int16_t  scale;
	int16_t  key;
	int16_t  tempo;
	int16_t  humanize;
	int16_t  octave_range;
	int16_t  channel;
};

struct uiSettings {
	uint16_t  on_off;
	uint16_t  scale;
	uint16_t  key;
	uint16_t  tempo_bpm;
	uint16_t  humanize;
	uint16_t  octave_range;
	uint16_t  channel;
};

extern int16_t menuIndex;
extern ui_state_t ui_state;
extern const char *menuNames[];
extern const size_t menuCount;

extern struct uiEncoderValues ui_encoderValues;
extern bool value_encoder_ignore_next;
extern struct uiSettings ui_settings;

void handle_menu_encoder(int16_t encoder_value, int16_t delta);
void handle_value_encoder(int16_t encoder_value, int16_t delta);

#endif /* INC_UI_H_ */
