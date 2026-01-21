/*
 * sequencer.h
 *
 *  Created on: Jan 20, 2026
 *      Author: dwask
 */

#ifndef INC_SEQUENCER_H_
#define INC_SEQUENCER_H_

#include "stdbool.h"

void play_random(uint32_t number_notes, uint16_t tempo, uint16_t range, bool send_note_off_flag);

#endif /* INC_SEQUENCER_H_ */
