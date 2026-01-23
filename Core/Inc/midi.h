/*
 * midi.h
 *
 *  Created on: Jan 20, 2026
 *      Author: dwask
 */

#ifndef INC_MIDI_H_
#define INC_MIDI_H_

#include <stdbool.h>

/* define data structure for a "frame" of midi data */
typedef struct{
    uint8_t running_status; /* MIDI uses “running status” to omit repeated status bytes */
	uint8_t data[2]; /* data payload (note/velocity/etc) */
} stc_midi;

char *midi_process_message(uint8_t status, uint8_t data1, uint8_t data2);
void midi_get_note_name(uint8_t note_number, char* buffer);

#endif /* INC_MIDI_H_ */
