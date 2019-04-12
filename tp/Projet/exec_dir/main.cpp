#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include "minuterie.h"

using minuterie;

PORTC = 0xFF;

int main() {
    MINUTERIE timer;
    PORTC = 0xFF;
    timer.startMinuterie(0xFFFF);
    while(TCNT2 < 0x5FFF)
    {}

    PORTC = 0xFF;


    

}

