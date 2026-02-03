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


