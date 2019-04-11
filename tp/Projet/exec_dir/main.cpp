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
    uint8_t ralenti = (rapport >> 2);

    enum etats {LIGNE, COURBE1, COURBE2};
    etats etat = LIGNE;
    uint8_t valueMap;
    
    uint32_t compteurG = 0;
    uint32_t compteurD = 0;

    for(;;){
    
        lineTracker.updateValueMap();
        valueMap = lineTracker.getValueMap();
        PORTC = valueMap;

        pwm.avancementAjuste(rapport, valueMap);
        
        
    }
}

