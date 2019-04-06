#ifndef F_CPU
#define F_CPU 8000000UL
#endif
 
// #include <util/atomic.h>
// #include <util/delay.h>
// #include "del.h"
// #include "debug.h"
// #include "util.h"

#include "sirc.h"

int main() {
    DEBUG_INFO((uint8_t*)"START OF PROGRAM");

//Port Setup

    DDRD = 0xFF;
     
//PWM Setup

    setupSIRC(); //enable/disable as you need

//Global interrupt setup

    sei();

//Test function is below

    //dummy command
    uint8_t command = 0x01;
    //dummy address
    uint8_t address = 0x02; //this will get ignored if in MODE_1

    //Tested function

    transmit(command, address);

    DEBUG_INFO((uint8_t*)"END OF PROGRAM");
}

