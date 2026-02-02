/*
 * presets.c
 *
 *  Created on: Feb 2, 2026
 *      Author: dwask
 */

#include "presets.h"

#define X(name, display, key, mode, low, high) \
    { display, key, mode, low, high },

const preset_t presets[] = {
#include "presets_def.h"
};
#undef X

//
//const char *presets_display_names[] = {"Peaceful", "Melodic", "Gritty", "Cinematic"};
//
//const uint16_t presets_count = sizeof(presets_display_names)/sizeof(presets_display_names[0]);


