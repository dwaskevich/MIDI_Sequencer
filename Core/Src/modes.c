/*
 * modes.c
 *
 *  Created on: Feb 1, 2026
 *      Author: dwask
 */

#include "modes.h"

const char *mode_display_names[] = {
#define X(name, disp, count, i1,i2,i3,i4,i5,i6,i7,i8) disp,
#include "mode_def.h"
#undef X
};

const uint8_t mode_interval_count[] = {
#define X(name, disp, count, i1,i2,i3,i4,i5,i6,i7,i8) count,
#include "mode_def.h"
#undef X
};

const uint8_t mode_intervals[][8] = {
#define X(name, disp, count, i1,i2,i3,i4,i5,i6,i7,i8) {i1,i2,i3,i4,i5,i6,i7,i8},
#include "mode_def.h"
#undef X
};

