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
extern volatile uint8_t pointCounterISR;
extern volatile uint8_t cornerCounterISR;
extern volatile bool useCornerISR;
extern volatile bool usePointISR;

#define petit 0x01 //dummy values
#define grand 0x02 //dummy values




ISR(INT0_vect){
    _delay_ms(30);
    
    if(PIND & 0x04){
        boolISR = true;
        useCornerISR = true;
        usePointISR = true;
        pointCounterISR++;
        cornerCounterISR++;
        intWaitISR = true;
    }
    DEBUG_PARAMETER_VALUE((uint8_t*)"receive",(void*) &cornerCounterISR);


}

static uint8_t cmd = 0x00;

ISR(PCINT2_vect){
    if(prev_pin_value == 0x20){
        //edge from hi to lo
        prev_pin_value = 0x00;
        // //VERIFY HEADER!
        headerDetected = verifyHeader();
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
  DDRD = 0xF0;
    
   PORTC = 0x04;
   _delay_ms(3000);
   PORTC = 0x00;

    uint8_t tmp = PINC & 0x20;
    prev_pin_value = tmp;
    sei();
   enablePCINT();
   while(!headerDetected){}
    headerDetected = false;
    disablePCINT();
    PORTB = 0x02;
    PORTC = cmd;
      _delay_ms(3000);
   PORTC = 0x00;

    Robot robot;
   robot.isr_INIT();
   robot.Run(cmd-1);

    }





