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
    
    
    DDRC = 0xFF;
    DDRB = 0xFF;

    uint8_t rapport = pwm.getVitesseDefault();
    uint8_t duree = 0xFF;
    
    enum etats {INIT, DEUX, TROIS, QUATRE,CINQ, SIX, SEPT,FIN};
   //enlever milieu 
    enum aStates {IR_WAIT, P1P2P3 ,P4P5P6, P7P8P9, lol, GOTO_S3};
    uint32_t rapport3Inch = 0;
    etats etat = INIT;
    uint16_t temps;
    uint8_t random = 0x00;

    //IMPORTANT À CHANGER **************************`````
    aStates state = P1P2P3;
    uint8_t valueMap;
    

    for(;;){
    
        lineTracker.updateValueMap();
        uint8_t valueMap = lineTracker.getValueMap();
        PORTC = valueMap;
        //DEBUG_PARAMETER_VALUE((uint8_t*)
        switch(etat){
            case INIT:
                startMinuterie(duree);
                pwm.avancer(rapport);
                
                temps = TCNT1;
                if(valueMap == 0x1F){
                    pwm.arreterMilieuLigne();
                    
                    temps = TCNT1;
                    rapport3Inch =  (temps >> 1) - (temps >> 4);
                    stopMinuterie();
                    resetMinuterie();

                   
                    etat = DEUX;
                }
            break;

            case DEUX:
                startMinuterie(duree);
                pwm.avancer(rapport);
                if(TCNT1 > 2*rapport3Inch){
                    pwm.arreterMilieuLigne();
                    etat = TROIS;
                    stopMinuterie();
                    resetMinuterie();
                }
                    
                

            break;
            

            case TROIS:
                pwm.tourner90Gauche(rapport);
                etat = QUATRE;
            break;

            case QUATRE:
                startMinuterie(duree);
                pwm.avancer(rapport);
                if(TCNT1 > 3*rapport3Inch){
                    etat = CINQ;
                    stopMinuterie();
                    resetMinuterie();
                }
            break;

            case CINQ:
                pwm.tourner90Droite(rapport);
                _delay_ms(3000);
                pwm.tourner90Droite(rapport);
                etat = SIX;
            break;

            case SIX:
                startMinuterie(duree);
                pwm.avancer(rapport);
                if(TCNT1 > 3*rapport3Inch){
                    etat = SEPT;
                    stopMinuterie();
                    resetMinuterie();
                }
            break;

            case SEPT:
                pwm.tourner90Gauche(rapport);
                pwm.arreterMilieuLigne();
                etat = FIN;
            break;


            

           
        }
        
    }
}

