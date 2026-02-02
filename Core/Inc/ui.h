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
#define X(name, str) MENU_##name,
#include "ui_menu_def.h"
#undef X
    MENU_COUNT
} menu_id_t;

typedef enum {
	MAJOR,
	MINOR
} eScale_t;

enum {
	OFF,
	ON
};

struct uiEncoderValues {
	int16_t  menu_encoder_previous_value;
	int16_t  value_encoder_previous_value;
	int16_t  on_off;
	int16_t  mode;
	int16_t  key;
	int16_t  tempo;
	int16_t  humanize;
	int16_t  octave_range_low;
	int16_t  octave_range_high;
	int16_t  channel_low;
	int16_t  channel_high;
	int16_t  presets;
};

struct uiSettings {
	uint16_t  on_off;
	uint16_t  mode;
	uint16_t  key;
	uint16_t  tempo_bpm;
	uint16_t  humanize;
	uint16_t  octave_low;
	uint16_t  octave_high;
	int16_t   channel_low;
	int16_t   channel_high;
	int16_t   presets;
};

extern int16_t menuIndex;
extern const char *menuNames[];
extern const size_t menuCount;

extern struct uiEncoderValues ui_encoderValues;
extern bool value_encoder_ignore_next;
extern struct uiSettings ui_settings;

extern bool ui_heartbeat_display_update_flag;
extern bool ui_primary_secondary_value_flag;

extern char ui_display_buffer_a[16];
extern char ui_display_buffer_b[16];

void handle_menu_encoder(int16_t encoder_value, int16_t delta);
void handle_value_encoder(int16_t encoder_value, int16_t delta);

#endif /* INC_UI_H_ */
