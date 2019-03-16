#ifndef LIB_DIR_MINUTERIE_H
#define LIB_DIR_MINUTERIE_H

#include <avr/io.h> 
#include <util/delay.h>

const int MINUTERIEEXPIRE = 0;

void startMinuterie(uint16_t duree);
void resetMinuterie ();

#endif 
