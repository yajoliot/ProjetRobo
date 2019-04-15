#ifndef LIB_DIR_MINUTERIE_H
#define LIB_DIR_MINUTERIE_H

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h> 
#include <util/delay.h>

const int MINUTERIEEXPIRE = 0;

void startMinuterie(uint16_t duree);
void stopMinuterie();
void resetMinuterie ();
void resetRegisters();
void startMinuterieIR();

void stopMinuterieIR();
void resetMinuterieIR();
void setPrescalerIR(uint8_t pos);

#endif 
