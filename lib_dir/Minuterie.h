#ifndef LIB_DIR_ANTIREBOND_H
#define LIB_DIR_ANTIREBOND_H

#include <avr/io.h> 
#include <util/delay.h>

const int MINUTERIEEXPIRE = 0;

void startMinuterie(uint16_t duree);
void resetMinuterie (uint16_t duree);

#endif LIB_DIR_ANTIREBOND_H
