/*
 * notes.c
 *
 *  Created on: Feb 1, 2026
 *      Author: dwask
 */

#include "notes.h"
#include <stdio.h>
#include "modes.h"


#include "ui.h"

//const uint8_t major_intervals[7] = { 2, 2, 1, 2, 2, 2, 1 };
//const uint8_t minor_intervals[7] = { 2, 1, 2, 2, 1, 2, 2 };

const uint8_t note_semitone[NOTE_COUNT] = {
#define X(name, semitone) semitone,
#include "notes_def.h"
#undef X
};

static const char *note_display_names[] = {
    "C", "C#", "D", "D#", "E", "F",
    "F#", "G", "G#", "A", "A#", "B"
};

// Builds a note selection list (i.e. note universe) into output[].
// key/pitch class 0–11 (C=0, C#=1, ..., B=11)
// intervals: references arrays holding intervals (i.e. semitone steps for the selected mode)
// low/high: sets octave range and therefore number of notes in universe (C4 convention ... add 1 to get accurate midi note decimal value)
// max_notes: size of array that will hold output ... NOTE: return value will indicate actual number of notes in constructed universe
int16_t build_scale(uint8_t pitch_class,
                const uint8_t *semitone_interval,
				uint8_t interval_count,
                uint8_t octave_low,
                uint8_t octave_high,
                uint8_t *output_note_array,
                int max_notes)
{
    uint8_t root_note = ((octave_low + 1) * 12) + pitch_class; /* sets lowest MIDI note in selection list */
    uint8_t note = root_note;
    int16_t count = 0;

    /* verify output on console */
    printf("%s (%s_%d, %d octaves): ", mode_display_names[ui_settings.mode], note_display_names[pitch_class], octave_low, octave_high - octave_low);
    while (note <= (((octave_high + 1) * 12) + pitch_class) && count < max_notes) {
    	printf("%d, ", note);
    	output_note_array[count++] = note;
        uint8_t interval = semitone_interval[(count - 1) % interval_count]; /* modulo automatically circles back to beginning of semitone sequence array */
        note += interval; /* advance note by referenced number of semitones (i.e. full-step or half-step) */
    }
    printf(" - #notes = %d\r\n", count);

    return count; /* number of notes generated ... use return value for traversing the note list array */
}

const char *note_to_string(uint8_t midi)
{
    uint8_t semitone = midi % 12;
    return note_display_names[semitone];
}

