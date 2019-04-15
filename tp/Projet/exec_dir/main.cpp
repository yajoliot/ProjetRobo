#ifndef F_CPU
 #define F_CPU 8000000UL
 #endif
 
 #include <util/atomic.h>
 #include <util/delay.h>
 #include <stdlib.h>
 #include <avr/interrupt.h>
 #include "pwm.h"
 #include "piezo.h"
 #include "USART.h"
 #include "memoire_24.h"
 #include "bytecode.h"
 #include "util.h"
 #include "LineTracker.h"
 #include "debug.h"
 #include "Minuterie.h"
 #include "Robot.h"
 #include "sirc.h"


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
         pointCounterISR++;
         cornerCounterISR++;
         useCornerISR = true;
         usePointISR = true;
    }


}

static uint8_t cmd = 0x00;

ISR(PCINT2_vect){
    if(prev_pin_value == 0x20){
        //edge from hi to lo
        prev_pin_value = 0x00;
        // //VERIFY HEADER!
        headerDetected = headerVerified();
        if(headerDetected){
            cmd = readBits(COMMAND_SIZE);
        }
    }else/*prev_pin_value == 0x00*/{
        // highEdge = true;
        prev_pin_value = 0x20;
    }
}


int main() {
    DDRA = 0x00;
DDRB = 0xFF;
DDRC = 0xDF;
DDRD = 0xFB;
   


   PORTC = 0x04;
   _delay_ms(3000);
   PORTC = 0x00;

    uint8_t tmp = PINC & 0x20;
    prev_pin_value = tmp;
    sei();
   enablePCINT();
   while(!headerDetected){}
    disablePCINT();
    PORTB = 0x02;
    PORTC = cmd;
      _delay_ms(3000);
   PORTC = 0x00;

    Robot robot;
   robot.isr_INIT();
   robot.Run(cmd);
   // uint8_t tmp = robot.receive();
   // PORTC = tmp;
   // robot.Run(tmp);
   for(;;){}
}
