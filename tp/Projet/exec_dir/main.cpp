#ifndef F_CPU
 #define F_CPU 8000000UL
 #endif
 
 #include <util/atomic.h>
 #include <util/delay.h>
<<<<<<< HEAD
 #include <avr/interrupt.h>
 #include "pwm.h"
=======
 #include <pwm.h>
>>>>>>> SuiveurDeLigne
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
<<<<<<< HEAD
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
    
=======
    enum etats {LIGNE_DROITE, TOURNE_GAUCHE, TOURNE_DROITE, PRE_BOITE, BOITE, POST_BOITE, ARRETE};
    int etat = LIGNE_DROITE;
    DDRC = 0xFF;
    DDRB = 0xFF;
    uint8_t rapport = pwm.getVitesseDefault(); // à changer pas bon -xavier
    bool boolBoite = false;
    bool tournerGauche = false;
    bool tournerDroite = false;
>>>>>>> SuiveurDeLigne

    for(;;){
        lineTracker.updateValueMap();
<<<<<<< HEAD
        if(getValueMap() == 0x1F){
            etat = INTWAIT;
=======
        uint8_t valueMap = lineTracker.getValueMap();
        PORTC = valueMap;
        

        //TODO:  mettre dans une fonction
        if((valueMap == 4 || 
            valueMap == 6 ||
            valueMap == 12 || 
            valueMap == 8 || 
            valueMap == 2 || 
            valueMap == 3 || 
            valueMap == 24 ||
            valueMap == 1 || 
            valueMap == 16) && 
            !boolBoite){
            etat = LIGNE_DROITE;
            // tournerGauche = false;
            // tournerDroite = false;
        }
        else if((valueMap == 7 || valueMap == 15 || (valueMap == 0 && tournerGauche)) && !boolBoite){
            etat = TOURNE_GAUCHE;
            tournerGauche = true;
            tournerDroite = false;
        }
        else if((valueMap == 28 || valueMap == 30 || (valueMap == 0 && tournerDroite)) && !boolBoite){
            etat = TOURNE_DROITE;
            tournerGauche = false;
            tournerDroite = true;
        }
        
        //TODO: AJOUTER LE CAS VALUEMAP == 0
        
        //Conditions pour entrer dans la boite
        else if (valueMap == 31 && !boolBoite) {
            
            etat = PRE_BOITE;
        }
        DEBUG_PARAMETER_VALUE((uint8_t*)"IF STATEMENTS", &valueMap);
        
        //TODO:  mettre dans une fonction
        switch(etat){
            case LIGNE_DROITE:
                    pwm.avancementAjuste(rapport, valueMap);
                break;
            
            case TOURNE_GAUCHE:
                    //TODO:  mettre dans une fonction
                    pwm.tournantGauche(rapport, valueMap);
                break;

            case TOURNE_DROITE:
                    //TODO:  mettre dans une fonction
                    pwm.tournantDroite(rapport, valueMap);
                break;

            case PRE_BOITE:
                    //TODO: à mettre dans fonction à part
                
                    while(lineTracker.getValueMap() == 31){
                        uint8_t temporaire = lineTracker.getValueMap();
                        DEBUG_PARAMETER_VALUE((uint8_t*)"PRE_BOITE", &temporaire);
                        pwm.avancer(pwm.getVitesseDefault());
                        lineTracker.updateValueMap();
                    }
                    boolBoite = true;
                    etat = BOITE;
                break;

            case BOITE : 
                    //TODO: à mettre dans fonction à part
                    while( !(lineTracker.getValueMap() == 31) ){
                        uint8_t temporaire = lineTracker.getValueMap();
                        DEBUG_PARAMETER_VALUE((uint8_t*)"BOITE", &temporaire);
                        pwm.boite(rapport, temporaire);
                        lineTracker.updateValueMap();
                    }
                    etat = POST_BOITE;
                    
                break;

            case POST_BOITE:
                    while(lineTracker.getValueMap() == 31){
                        uint8_t temporaire = lineTracker.getValueMap();
                        DEBUG_PARAMETER_VALUE((uint8_t*)"POST_BOITE", &temporaire);
                        pwm.avancer(pwm.getVitesseDefault());
                        lineTracker.updateValueMap();
                    }
                    boolBoite = false;
                    etat = LIGNE_DROITE;
                break;
            
            case ARRETE:
                    pwm.arreter();
                break;
>>>>>>> SuiveurDeLigne
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



