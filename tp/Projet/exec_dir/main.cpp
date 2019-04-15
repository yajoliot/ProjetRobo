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

ISR(PCINT2_vect){
    if(prev_pin_value == 0x20){
        //edge from hi to lo
        prev_pin_value = 0x00;
        // //VERIFY HEADER!
        
        if(verifyHeader()){
            uint8_t tmp;
            tmp = readBits(COMMAND_SIZE);
  
            PORTD = tmp;
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
    

    Robot robot;

    //sei();
    //enablePCINT();

    robot.isr_INIT();
    
    uint8_t tempIR = robot.receive();
   

    robot.Run(tempIR);

    

    }





