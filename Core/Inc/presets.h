/*
 * presets.h
 *
 *  Created on: Feb 2, 2026
 *      Author: dwask
 */

#ifndef INC_PRESETS_H_
#define INC_PRESETS_H_

//#include <stdint.h>

#pragma once
#include "notes.h"
#include "modes.h"

typedef struct {
    const char *name;
    uint8_t key;
    uint8_t mode;
    uint8_t octave_low;
    uint8_t octave_high;
} preset_t;

#define X(name, display, key, mode, low, high) PRESET_##name,
typedef enum {
#include "presets_def.h"
    PRESET_COUNT
} preset_name_t;
#undef X

extern const preset_t presets[];


//enum {
//	PEACEFUL,
//	MELODIC,
//	GRITTY,
//	CINEMATIC
//};
//
//extern const char *presets_display_names[];
//extern const uint16_t presets_count;

#endif /* INC_PRESETS_H_ */
