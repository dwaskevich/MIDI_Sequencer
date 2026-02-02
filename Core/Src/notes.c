/*
 * notes.c
 *
 *  Created on: Feb 1, 2026
 *      Author: dwask
 */

#include "notes.h"
#include <stdio.h>

const uint8_t major_intervals[7] = { 2, 2, 1, 2, 2, 2, 1 };
const uint8_t minor_intervals[7] = { 2, 1, 2, 2, 1, 2, 2 };

const uint8_t note_semitone[NOTE_COUNT] = {
#define X(name, semitone) semitone,
#include "notes_def.h"
#undef X
};

static const char *note_display_names[] = {
    "C", "C#", "D", "D#", "E", "F",
    "F#", "G", "G#", "A", "A#", "B"
};

// Builds a note list universe into output[].
// root_midi: e.g. C, D, etc
// intervals: references arrays holding major scale intervals or minor scale intervals (i.e. semitone sequences)
// low/high: octave (C4 convention ... add 1 to get accurate midi note decimal value)
// max_notes: size of array that will hold output ... NOTE: return value will indicate actual number of notes in constructed universe
int16_t build_scale(uint8_t root_midi_note,
                const uint8_t *semitone_interval,
                uint8_t octave_low,
                uint8_t octave_high,
                uint8_t *output_note_array,
                int max_notes)
{
    uint8_t note = ((octave_low + 1) * 12) + root_midi_note;
    int16_t count = 0;

    while (note <= (((octave_high + 1) * 12) + root_midi_note) && count < max_notes) {
    	printf("%d, ", note);
    	output_note_array[count++] = note;
        uint8_t interval = semitone_interval[(count - 1) % 7]; /* modulo automatically circles back to beginning of semitone sequence array */
        note += interval; /* advance note by referenced number of semitones (i.e. full-step or half-step) */
    }
    printf("\r\n");

    return count; /* number of notes generated ... use return value for traversing the note list array */
}

const char *note_to_string(uint8_t midi)
{
    uint8_t semitone = midi % 12;
    return note_display_names[semitone];
}

