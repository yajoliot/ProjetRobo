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
enum etats2 {INIT2,ANTI_REBOND, DIST_1, DIST_2};
volatile etats etat = INIT;
volatile etats2 etat2 = INIT2;


#define petit 0x01 //dummy values
#define grand 0x02 //dummy values

void isr_INIT() {

    DDRD = 0x00;
    DDRB = 0xff;

    
    cli ();

    EIMSK |= (1 << INT0);

    EICRA |= (1 << ISC01) | (1 << ISC00); // EICRA = EICRA | (1 << ISC01)

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
    uint8_t rapport = 150;
    uint16_t duree  = 0xFFFF;
    uint16_t distTime1 = 0;
    uint16_t distTime2 = 0;
    uint16_t compare_value;
    bool droite = true;
    bool analyse = false;
    


    for(;;){

        lineTracker.updateValueMap();
        PORTC = lineTracker.getValueMap();
        if(lineTracker.getValueMap() == 0x1F && !analyse){
            etat = INTWAIT;
            analyse = true;
        }
        

        switch(etat){
            case INIT:
                pwm.avancementAjuste(rapport, lineTracker.getValueMap());
                break;

            case INTWAIT:
                pwm.arreter();
                break;

            case ANALYSE:
                if(lineTracker.getValueMap() == 0x1F){
                    etat2 = ANTI_REBOND;
                }
                    
                switch(etat2){
                    case INIT2:
                        pwm.avancementAjuste(rapport, lineTracker.getValueMap());
                    break;

                    case ANTI_REBOND:
                        pwm.avancementAjuste(rapport, lineTracker.getValueMap());
                        if(lineTracker.getValueMap() == 0x04 || lineTracker.getValueMap() == 0x06 || lineTracker.getValueMap() == 0x0C){
                            etat2 = DIST_1;
                        }
                    break;

                    case DIST_1:
                        startMinuterie(duree);
                        while(!(lineTracker.getValueMap() == 0x1C
                            || lineTracker.getValueMap() == 0x07 || lineTracker.getValueMap() == 0x06 || lineTracker.getValueMap() == 0x0C)){
                            lineTracker.updateValueMap();
                            PORTC = lineTracker.getValueMap();
                            pwm.avancementAjuste(rapport, lineTracker.getValueMap());
                            uint8_t tmp = lineTracker.getValueMap();        
                            DEBUG_PARAMETER_VALUE((uint8_t*)"test",&tmp);
                        }
                        if(lineTracker.getValueMap() == 0x07) //if found left
                            droite = false;

                        distTime1 = TCNT1;
                        etat2 = DIST_2;
                    break;

                    case DIST_2:
                        startMinuterie(duree);
                        if(droite){
                            while(!(lineTracker.getValueMap() == 0x07)){
                                lineTracker.updateValueMap();
                                pwm.avancementAjuste(rapport, lineTracker.getValueMap());
                            }

                        } else {
                            while(!(lineTracker.getValueMap() == 0x1C)){
                                lineTracker.updateValueMap();
                                pwm.avancementAjuste(rapport, lineTracker.getValueMap());
                            }
                        }

                        distTime2 = TCNT1;
                        etat = WAIT_TILL_END;
                        
                    break;
                    
                }
                
                
            break;

            case WAIT_TILL_END:
                if(lineTracker.getValueMap() == 0x00){
                    etat = END;
                    pwm.arreter();
                }
                compare_value = (distTime1>>1)+(distTime1>>3);
            break;

            case END:
                pwm.arreter();

                uint8_t tmp;
                DEBUG_PARAMETER_VALUE((uint8_t*)"droite",&droite);
                tmp = compare_value >> 8;
                DEBUG_PARAMETER_VALUE((uint8_t*)"0.5 distTime1 hi",&tmp);
                tmp = compare_value;
                DEBUG_PARAMETER_VALUE((uint8_t*)"0.5 distTime1 lo",&tmp);
                tmp = (abs(distTime2 - distTime1)>>8);
                DEBUG_PARAMETER_VALUE((uint8_t*)"difference hi",&tmp);
                tmp = abs(distTime2 - distTime1);
                DEBUG_PARAMETER_VALUE((uint8_t*)"difference lo",&tmp);
                bool tmpb = abs(distTime2 - distTime1) < compare_value && droite;
                DEBUG_PARAMETER_VALUE((uint8_t*)"cas1",&tmpb);
                tmpb = abs(distTime2 - distTime1) > compare_value && droite;
                DEBUG_PARAMETER_VALUE((uint8_t*)"cas2",&tmpb);
                tmpb = abs(distTime2 - distTime1) < compare_value && !droite;
                DEBUG_PARAMETER_VALUE((uint8_t*)"cas3",&tmpb);
                tmpb = abs(distTime2 - distTime1) > compare_value && !droite;
                DEBUG_PARAMETER_VALUE((uint8_t*)"cas4",&tmpb);

                if( abs(distTime2 - distTime1) < compare_value  && droite){ 
                    PORTC = 0x10;
                } else if ( abs(distTime2 - distTime1) > compare_value && droite) {
                    PORTC = 0x08;
                } else if( abs(distTime2 - distTime1) < compare_value && !droite ) {
                    PORTC = 0x04;
                } else {
                    PORTC = 0x02;
                } 
                
                for(;;){}
        
            break;


        }

        
        
    }
}



