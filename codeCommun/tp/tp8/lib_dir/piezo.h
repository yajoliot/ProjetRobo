#ifndef LIB_DIR_PIEZO_H
#define LIB_DIR_PIEZO_H

#include <avr/io.h>
#include "notes.h"
#include "util.h"
#include "debug.h"

//WARNING
//USE THESE FUNCTIONS BEFORE USING THE ONES AT THE BOTTOM.
//THE FUNCTIONS THAT DO NOT APPEAR HERE CAN BE USED
#define PIEZO_INIT(...) piezoInit(VARIADIC_THREE(__VA_ARGS__, MAX_VOLUME))
#define PLAY_NOTE(...) playNote(VARIADIC_TWO(__VA_ARGS__, DEFAULT_SOUND_DURATION_IN_MS))

const uint8_t MAX_VOLUME = 100;
const uint16_t DEFAULT_SOUND_DURATION_IN_MS = 0;

//DO NOT USE THESE DIRECTLY
void piezoInit(uint8_t pwm_pin, uint8_t ground_pin, uint8_t volume);
void playNote(uint8_t midi_key, uint16_t duration_in_ms);

//YOU CAN USE THESE DIRECTLY 
void setVolume(uint8_t volume);

#endif