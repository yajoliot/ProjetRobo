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
 #include "Robot.h"
 #include "Sirc.h"


extern volatile bool boolISR;
extern volatile etats etat;
extern volatile uint8_t pointCounterISR;
extern volatile uint8_t cornerCounterISR;
extern volatile bool useCornerISR;
extern volatile bool usePointISR;

#define petit 0x01 //dummy values
#define grand 0x02 //dummy values


ISR(INT0_vect){
    _delay_ms(30);
    if(PIND & 0x04){
        etat = ANALYSE;
    }
    pointCounterISR++;
    cornerCounterISR++;
    useCornerISR = true;
    usePointISR = true;

}

ISR(PCINT2_vect){
    if(prev_pin_value == 0x20){
        //edge from hi to lo
        prev_pin_value = 0x00;
        // //VERIFY HEADER!
        headerDetected = headerVerified();
        if(headerDetected){
            uint8_t tmp;
            tmp = readBits(COMMAND_SIZE);
  
            PORTD = tmp;
            for(;;){_delay_ms(300); PORTB = 0x02;}
        }
    }else/*prev_pin_value == 0x00*/{
        // highEdge = true;
        prev_pin_value = 0x20;
    }
}


int main() {
   
   Robot robot;
   robot.isr_INIT();

   robot.Run(0x00);

}



