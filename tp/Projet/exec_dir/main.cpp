#ifndef F_CPU
 #define F_CPU 8000000UL
 #endif
 
 #include <util/atomic.h>
 #include <util/delay.h>
 #include <pwm.h>
 #include "piezo.h"
 #include "usart.h"
 #include "memoire_24.h"
 #include "usart.h"
 #include "bytecode.h"
 #include "util.h"
 #include "LineTracker.h"
 #include "debug.h"
 #include "Minuterie.h"



int main() {
    PWM pwm;
    LineTracker lineTracker;
    //PIEZO_INIT(DDD4, DDD5, 50); Le piezo Fuck avc TCCR1A de minuterie
    

    DDRC = 0xFF;
    DDRB = 0xFF;

    uint8_t rapport = pwm.getVitesseDefault();
    uint8_t ralenti = rapport >> 1;

    enum etats {LIGNE, COURBE, BLOCK, TOURNANTGAUCHE, TOURNANTDROIT};
    etats etat = LIGNE;
    uint8_t valueMap;
    
    uint8_t compteurG = 0;
    uint8_t compteurD = 0;

    for(;;){
    
        lineTracker.updateValueMap();
        uint8_t valueMap = lineTracker.getValueMap();
        PORTC = valueMap;

        if(compteurD > 3){
            etat = COURBE;
        } else if(compteurG > 3){
            etat = COURBE;
        } else if( valueMap == 7 ||
                   valueMap == 15 ){
            pwm.arreter();
            etat = TOURNANTGAUCHE;
        } else if( valueMap == 28 ||
                   valueMap == 30 ){
            pwm.arreter();
            etat = TOURNANTDROIT;
        }
        
        if( valueMap == 6 ){
            compteurG += 1;
            compteurD = 0;
        } else if( valueMap == 12 ){
            compteurD += 1;
            compteurG = 0;
        }

        switch(etat){
            case LIGNE:
                pwm.avancementLeger(rapport, valueMap);
            break;
            case TOURNANTGAUCHE:
                pwm.tourner90Precis(0, rapport);
                etat = LIGNE;
            break;

            case TOURNANTDROIT:
                pwm.tourner90Precis(1, rapport);
                etat = LIGNE;
            break;
            case COURBE:
                while(valueMap == )
                    pwm.avancementLeger(ralenti, valueMap);
                
            break;
            case BLOCK:
                startMinuterie(0xFF);
                while(TCNT1 < 0x1F){
                    lineTracker.updateValueMap();
                    valueMap = lineTracker.getValueMap();
                    pwm.avancementLeger(rapport, valueMap);
                }
                stopMinuterie();
                resetMinuterie();

                etat = LIGNE;

            break;
        }
        
        
    }
}

