/*
 * notes.h
 *
 *  Created on: Feb 1, 2026
 *      Author: dwask
 */

#ifndef INC_NOTES_H_
#define INC_NOTES_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
#define X(name, semitone) NOTE_##name,
#include "notes_def.h"
#undef X
    NOTE_COUNT
} note_name_t;

extern const uint8_t note_semitone[NOTE_COUNT];
extern const uint8_t major_intervals[7];
extern const uint8_t minor_intervals[7];

int16_t build_scale(uint8_t root_midi_note,
                const uint8_t *semitone_interval,
                uint8_t octave_low,
                uint8_t octave_high,
                uint8_t *output_note_array,
                int max_notes);

const char *note_to_string(uint8_t midi);

#endif /* INC_NOTES_H_ */
