#ifndef F_CPU
 #define F_CPU 8000000UL
 #endif

 #include <util/atomic.h>
 #include <util/delay.h>
 #include <stdlib.h>
 #include <avr/interrupt.h>
 #include "Robot.h"


extern volatile etats etat;
extern volatile bool boolISR;
extern volatile pointCounterISR;

//ISR POUR SECTION3 (RunCMD3())
ISR(INT0_vect){
    _delay_ms(30);
    if(PIND & 0x04){
        etat = ANALYSE;
        boolISR = !boolISR;
    }

    ++pointCounterISR;

    
}

int main() {
    Robot robot = Robot();
    
    robot.Run(0x03);

}