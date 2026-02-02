/*
 * mode_def.h
 *
 *  Created on: Feb 1, 2026
 *      Author: dwask
 */

// name, display_name, interval_count, i1,i2,i3,i4,i5,i6,i7,i8
X(IONIAN,     "Ionian",     7, 2,2,1,2,2,2,1, 0)
X(DORIAN,     "Dorian",     7, 2,1,2,2,2,1,2, 0)
X(PHRYGIAN,   "Phrygian",   7, 1,2,2,2,1,2,2, 0)
X(LYDIAN,     "Lydian",     7, 2,2,2,1,2,2,1, 0)
X(MIXOLYDIAN, "Mixolydi",   7, 2,2,1,2,2,1,2, 0)
X(AEOLIAN,    "Aeolian",    7, 2,1,2,2,1,2,2, 0)
X(LOCRIAN,    "Locrian",    7, 1,2,2,1,2,2,2, 0)

// Pentatonics (5 intervals)
X(PENT_MAJOR, "Pent Maj",   5, 2,2,3,2,3, 0,0,0)
X(PENT_MINOR, "Pent Min",   5, 3,2,2,3,2, 0,0,0)

// Blues (6 intervals)
X(BLUES,      "Blues",      6, 3,2,1,1,3,2, 0,0)

// Whole tone (6 intervals)
X(WHOLE_TONE, "WholeTon",   6, 2,2,2,2,2,2, 0,0)

X(HARM_MINOR, "Harm Min",   7, 2,1,2,2,1,3,1, 0)

X(MELODIC_MIN, "Melod Min", 7, 2,1,2,2,2,2,1, 0)

