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
 #include "linetracker.h"
 #include "debug.h"
 #include "Minuterie.h"

int main() {
    PWM pwm;
    LineTracker lineTracker;
    enum etats {INIT, TOURNANT_GAUCHE, DROIT, AJUSTEMENT, TOURNER};
    int etat = INIT;
    DDRC = 0xFF;
    DDRB = 0xFF;
    uint8_t rapport = pwm.getVitesseDefault(); // à changer pas bon -xavier
    bool boolBoite = false;
    bool tournerGauche = false;
    bool tournerDroite = false;
    uint8_t duree = 0xFF;
    uint8_t valueMap;
    uint16_t temps;

    for(;;){
     
     lineTracker.updateValueMap();
     PORTC = lineTracker.getValueMap();
     valueMap = lineTracker.getValueMap();

     switch(etat){

        case INIT:
            pwm.avancementAjuste(rapport, valueMap);
            if(valueMap == 7 || valueMap == 3 || valueMap == 15)
                etat = TOURNANT_GAUCHE;
        break;

        case TOURNANT_GAUCHE:
            pwm.avancer(pwm.getVitesseDefault());
            if(valueMap == 0)
                etat = DROIT;
        break;

        case DROIT :
            startMinuterie(duree);
            pwm.avancer(pwm.getVitesseDefault());

            if(valueMap != 0){
                etat = AJUSTEMENT;
                pwm.arreter();
                _delay_ms(3000);
                temps = TCNT1;
                stopMinuterie();
                resetMinuterie();
            }

        break;

        case AJUSTEMENT:
            pwm.avancer(pwm.getVitesseDefault());
            if(TCNT1 != ((temps>>1) + temps)) {
                etat = TOURNER;
            }
        break;

        case TOURNER:
            PORTC = 0x01;


     }

    }
}
    
