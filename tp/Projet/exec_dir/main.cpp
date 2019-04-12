#ifndef F_CPU
 #define F_CPU 8000000UL
 #endif

 #include <util/atomic.h>
 #include <util/delay.h>
 #include <stdlib.h>
 #include <avr/interrupt.h>
 #include "Robot.h"
 #include "Minuterie.h"


extern volatile etats etat;
 




int main() {
    PWM pwm;
    LineTracker lineTracker;
    DDRC = 0xFF;
    DDRB = 0xFF;
    enum etats{LIGNE, DROITE, GAUCHE , COURBE1, COURBE2, POSTCOURBE1};
    etats etat = LIGNE;
    uint8_t rapport = pwm.getVitesseDefault() + 50;
    uint8_t ralenti = rapport >> 2;
    uint8_t valueMap;
    uint32_t compteurG = 0;
    uint32_t compteurD = 0;
   for(;;){
       lineTracker.updateValueMap();
       valueMap = lineTracker.getValueMap();
       pwm.avancementAjuste(rapport, valueMap);

        
        if( valueMap == 7 || valueMap == 15 ) {
            pwm.tournantGauche(rapport, valueMap);
        } else if( valueMap == 28 || valueMap == 30 ) {
            pwm.tournantDroite(rapport, valueMap);
        }

        switch(etat){
            case LIGNE:
                pwm.avancementAjuste(rapport, valueMap);

                if(valueMap == 6 ||
                   valueMap == 2 ||
                   valueMap == 1 ){
                    compteurG++;
                } else if(valueMap == 8 ||
                          valueMap == 12 ||
                          valueMap == 24 ){
                    compteurG = 0;
                    compteurD++;
                }
                if(compteurG > 600)
                    etat = COURBE1;

            break;
            case COURBE1:

                startMinuterie(0xFFFF);
                while( TCNT1 < 0x7FFF){
                    PORTC = 0X00;
                    lineTracker.updateValueMap();
                    valueMap = lineTracker.getValueMap();
                    pwm.avancementAjuste(ralenti, valueMap);
                }
                stopMinuterie();
                resetMinuterie();
                compteurG = 0;
                etat = POSTCOURBE1;

            break;
            case POSTCOURBE1:
                startMinuterie(0xFFFF);
                while( TCNT1 < 0x3FFF){
                    lineTracker.updateValueMap();
                    valueMap = lineTracker.getValueMap();
                    PORTC = valueMap;
                    pwm.avancementAjuste(rapport, valueMap);
                }
                stopMinuterie();
                resetMinuterie();
                compteurG = 0;
                etat = LIGNE;


            break;
        }
       
   }

}