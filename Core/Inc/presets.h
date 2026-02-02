/*
 * presets.h
 *
 *  Created on: Feb 2, 2026
 *      Author: dwask
 */

#ifndef INC_PRESETS_H_
#define INC_PRESETS_H_

#include <stdint.h>

enum {
	PEACEFUL,
	MELODIC,
	GRITTY,
	CINEMATIC
};

extern const char *presets_display_names[];
extern const uint16_t presets_count;

#endif /* INC_PRESETS_H_ */
