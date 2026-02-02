/*
 * modes.h
 *
 *  Created on: Feb 1, 2026
 *      Author: dwask
 */

#ifndef INC_MODES_H_
#define INC_MODES_H_

#ifndef MODES_H
#define MODES_H

#include <stdint.h>

typedef enum {
#define X(name, disp, count, i1,i2,i3,i4,i5,i6,i7,i8) MODE_##name,
#include "mode_def.h"
#undef X
    MODE_COUNT
} mode_t;

extern const char *mode_display_names[MODE_COUNT];
extern const uint8_t mode_interval_count[MODE_COUNT];
extern const uint8_t mode_intervals[MODE_COUNT][8];

#endif


#endif /* INC_MODES_H_ */
