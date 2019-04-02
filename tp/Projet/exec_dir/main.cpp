#ifndef F_CPU
 #define F_CPU 8000000UL
 #endif
 
 #include <util/atomic.h>
 #include <util/delay.h>
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
    etat = ANALYSE;
}

ISR(TIM0_OVF_vect){
    timer += 1;
}

int main() {
    PWM pwm;
    LineTracker lineTracker;
    enum etats {INIT, INTWAIT, ANALYSE, };
    int etat = INIT;
    DDRC = 0xFF;
    DDRB = 0xFF;
    uint8_t rapport = 100;
    timer uint8_t = 0;
    duree uint16_t = ?;
    uint16_t distTime1 = 0;
    uint16_t distTime2 = 0;
    bool droite = true;
    

    for(;;){
        lineTracker.updateValueMap();
        if(getValueMap() == 0x1F){
            etat = INTWAIT;
        }
        

        switch(etat){
            case INIT:
                pwm.avancementAjuste(rapport, lineTracker.getValueMap())
            break;

            case INTWAIT:
                pwm.arreter();
            break;

            default:
            break;

            case ANALYSE:
                if(getValueMap() == 0x1F)
                    etat = DIST_1;
                    
                switch(etat){
                    case INIT:
                        pwm.avancementAjuste(rapport, lineTracker.getValueMap());
                    break;

                    case DIST_1:
                        startMinuterie();
                        while(!lineTracker.getValuemap() == 0x1C
                            || !lineTracker.getValuemap() == 0x07){
                            lineTracker.updateValueMap();
                        }
                        if(lineTracker.getValuemap() == 0x07)
                            droite = false;

                        distTime1 = timer;
                        timer = 0;
                        resetMinuterie();
                        etat = DIST_2;
                    break;
                    case DIST_2:
                        startMinuterie();
                        if(droite){
                            while(!lineTracker.getValuemap() == 0x07)
                                lineTracker.updateValueMap();
                        } else {
                            while(!lineTracker.getValuemap() == 0x1C)
                            lineTracker.updateValueMap();
                        }

                        distTime2 = timer;
                        timer = 0;
                        resetMinuterie();
                        
                    break;
                    
                }
                
                
            break;


        }

        
        
    }
}



