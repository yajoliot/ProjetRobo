#ifndef LIB_DIR_PIEZO_H
#define LIB_DIR_PIEZO_H

#include <avr/io.h>

void piezo_init(uint8_t location_value, uint8_t volume);
void setVolume(uint8_t volume);

#endif