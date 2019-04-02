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


enum etats {INIT, INTWAIT, ANALYSE, WAIT_TILL_END, END};
enum etats2 {INIT2, DIST_1, DIST_2};
volatile etats etat = INIT;
volatile etats2 etat2 = INIT2;

#define petit 0x01 //dummy values
#define grand 0x02 //dummy values

void isr_INIT() {

    DDRD = 0x00;
    DDRB = 0xff;

    
    cli ();

    EIMSK |= (1 << INT0);

    EICRA |= (1 << ISC01); // EICRA = EICRA | (1 << ISC01)

    sei();
}

ISR(INT0_vect){
    _delay_ms(30);
    if(PIND & 0x04){
        etat = ANALYSE;
    }
}


int main() {
    PWM pwm;
    LineTracker lineTracker;

    DDRC = 0xFF;
    DDRB = 0xFF;
    isr_INIT();
    uint8_t rapport = 100;
    uint16_t duree  = 0xFFFF;
    uint16_t distTime1 = 0;
    uint16_t distTime2 = 0;
    bool droite = true;
    


    for(;;){

        lineTracker.updateValueMap();

        if(lineTracker.getValueMap() == 0x1F){
            etat = INTWAIT;

        }
        

        switch(etat){
            case INIT:
                pwm.avancementAjuste(rapport, lineTracker.getValueMap());
                break;

            case INTWAIT:
                pwm.arreter();
                break;

            case ANALYSE:
                if(lineTracker.getValueMap() == 0x1F)
                    etat2 = DIST_1;
                    
                switch(etat2){
                    case INIT2:
                        pwm.avancementAjuste(rapport, lineTracker.getValueMap());
                    break;

                    case DIST_1:
                        startMinuterie(duree);
                        while(!(lineTracker.getValueMap() == 0x1C)
                            || !(lineTracker.getValueMap() == 0x07)){
                            lineTracker.updateValueMap();
                        }
                        if(lineTracker.getValueMap() == 0x07) //if found left
                            droite = false;

                        distTime1 = TCNT1;
                        etat2 = DIST_2;
                    break;

                    case DIST_2:
                        startMinuterie(duree);
                        if(droite){
                            while(!(lineTracker.getValueMap() == 0x07))
                                lineTracker.updateValueMap();
                        } else {
                            while(!(lineTracker.getValueMap() == 0x1C))
                                lineTracker.updateValueMap();
                        }

                        distTime2 = TCNT1;
                        etat = WAIT_TILL_END;
                        
                    break;
                    
                }
                
                
            break;

            case WAIT_TILL_END:
                if(lineTracker.getValueMap() == 0x00){
                    etat = END;
                }
            break;

            case END:
                
                pwm.arreter();


                if( abs(distTime2 - distTime1) < petit  && droite){
                    PORTC = 0x08;
                } else if ( abs(distTime2 - distTime1) < petit && !droite) {
                    PORTC = 0x02;
                } else if( abs(distTime2 - distTime1) > grand && droite) {
                    PORTC = 0x04;
                } else {
                    PORTC = 0x01;
                } 
                

        
            break;


        }

        
        
    }
}



