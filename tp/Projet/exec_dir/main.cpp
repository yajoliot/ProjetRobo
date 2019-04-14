#ifndef F_CPU
 #define F_CPU 8000000UL
 #endif
 
 #include <util/atomic.h>
 #include <util/delay.h>
 #include <stdlib.h>
 #include <avr/interrupt.h>
 #include "pwm.h"
 #include "piezo.h"
 #include "usart.h"
 #include "memoire_24.h"
 #include "usart.h"
 #include "bytecode.h"
 #include "util.h"
 #include "linetracker.h"
 #include "debug.h"
 #include "Minuterie.h"


extern volatile bool boolISR;
extern volatile etats etat;
extern volatile uint8_t pointCounterISR;
extern volatile uint8_t cornerCounterISR;

#define petit 0x01 //dummy values
#define grand 0x02 //dummy values


ISR(INT0_vect){
    _delay_ms(30);
    if(PIND & 0x04){
        etat = ANALYSE;
    }
    pointerCounter++;
    cornerCounter++;

}


int main() {
   
   uint8_t cornerValue = 0;
   Robot robot;
   isr_INIT();
   cornerValue = robot.receive();

   robot.Run(cornerValue);

}



