#ifndef LIB_DIR_PIEZO_H
#define LIB_DIR_PIEZO_H

#include <avr/io.h>
#include "notes.h"
#include "util.h"

//custom variadic function defines for piezo
//source:https://stackoverflow.com/a/9180314
#define VARIADIC_TWO(a, b, ...) (a), (b)
#define PIEZO_INIT(...) piezoInit(VARIADIC_TWO(__VA_ARGS__, MAX_VOLUME))
#define PLAYNOTE(...) playNote(VARIADIC_TWO(__VA_ARGS__, DEFAULT_SOUND_DURATION_IN_MS))

const uint8_t MAX_VOLUME = 100;
const uint16_t DEFAULT_SOUND_DURATION_IN_MS = 1000;

void piezoInit(uint8_t location_value, uint8_t volume);
void setVolume(uint8_t volume);
void playNote(uint8_t midi_key, uint16_t duration_in_ms);

#endif