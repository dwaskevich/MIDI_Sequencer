/*
 * midi.c
 *
 *  Created on: Jan 20, 2026
 *      Author: dwask
 */

#include "stdint.h"
#include "midi.h"
#include "display.h"
#include "stdio.h"
#include "string.h"

//#define MAX_LINES 8
#define MAX_CHARS 22  // 21 visible + null terminator

stc_midi midi_packet;

uint8_t midi_status = 0;
uint8_t midi_data[2];
uint8_t data_index = 0;

// Lookup table for MIDI note names
const char* note_names[] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
};

char *midi_process_message(uint8_t status, uint8_t data1, uint8_t data2)
{
    uint8_t channel = (status & 0x0F) + 1; // MIDI channels are 1–16
    uint8_t type = status & 0xF0;

    midi_packet.running_status = status;
    midi_packet.data[0] = data1;
    midi_packet.data[1] = data2;

    char note_str[5];
    static char line[MAX_CHARS];
    midi_get_note_name(data1, note_str);

    if (strlen(note_str) == 3)
        strcat(note_str, " ");

    switch (type) {
        case 0x90:
            if (data2 > 0)
                snprintf(line, MAX_CHARS, "On  Ch%2d %s V%d", channel, note_str, data2);
            else
                snprintf(line, MAX_CHARS, "Off Ch%2d %s", channel, note_str);
            break;
        case 0x80:
            snprintf(line, MAX_CHARS, "Off Ch%2d %s V%d", channel, note_str, data2);
            break;
        case 0xB0:
            snprintf(line, MAX_CHARS, "CC Ch%2d %d=%d", channel, data1, data2);
            break;
        default:
            snprintf(line, MAX_CHARS, "%02X %02X %02X", status, data1, data2);
            break;
    }

    return line;
}

// Convert MIDI note number (0–127) to string like "C4"
void midi_get_note_name(uint8_t note_number, char* buffer)
{
    uint8_t octave = (note_number / 12) - 1;   // MIDI note 60 = C4
    uint8_t index  = note_number % 12;
    sprintf(buffer, "%s_%d", note_names[index], octave);
}
