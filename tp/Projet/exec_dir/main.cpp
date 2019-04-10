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
    

    DDRA = 0x00;
    DDRC = 0xFF;
    DDRB = 0xFF;

    uint8_t rapport = pwm.getVitesseDefault();
    uint8_t duree = 0xFF;

    enum etats {INIT, ANALYSE_IR, WAIT, GOTO_S3}
    enum aStates {IR_WAIT, P1P2P3 ,P4P5P6, P7P8P9}
    uint32_t rapport3Inch;
    etats etat = INIT;
    aStates etat_analyze = IR_WAIT;
    uint8_t valueMap;

    //tempSirc temp replacemtn for ir receiver
    uint8_t tempSirc = 0x00;
    

    for(;;){
    
        lineTracker.updateValueMap();
        uint8_t valueMap = lineTracker.getValueMap();
        PORTC = valueMap;

        switch(etat){
            case INIT:
                pwm.avancementAjuste(rapport, valueMap);
                startMinuterie(duree);
                if(valueMap == 0x1F){
                    pwm.arreter();
                    rapport3Inch = (TCNT1 >> 1)
                    etat = ANALYSE_IR;
                    stopMinuterie();
                    resetMinuterie();
                }

            break;

            case ANALYSE_IR:

                switch(etat_analyze){

                    case IR_WAIT:
                    if(tempSirc == 0x00 ||
                    tempSirc == 0x01 ||
                    tempSirc == 0x02 ){
                        
                        etat_analyze = P1P2P3;
                    } else if (tempSirc == 0x03 ||
                    tempSirc == 0x04 ||
                    tempSirc == 0x05 ) {
                        etat_analyze = P4P5P6;
                    } else {
                        etat_analyze = P7P8P9;
                    break;

                    case P1P2P3:
                        startMinuterie(duree);
                        while(TCNT1 < 2*rapport3Inch)
                            pwm.avancer(rapport);
                        stopMinuterie();
                        resetMinuterie();

                        if(tempSirc == 0x00){
                            startMinuterie(duree);
                            while(TCNT1 < 4*rapport3Inch)
                                pwm.avancer(rapport);
                            stopMinuterie();
                            resetMinuterie();
                        } else if(tempSirc == 0x01){
                            startMinuterie(duree);
                            while(TCNT1 < 3*rapport3Inch)
                                pwm.avancer(rapport);
                            stopMinuterie();
                            resetMinuterie();
                        } else {
                            startMinuterie(duree);
                            while(TCNT1 < 2*rapport3Inch)
                                pwm.avancer(rapport);
                            stopMinuterie();
                            resetMinuterie();

                    break;

                    case P4P5P6:
                        startMinuterie(duree);
                        while(TCNT1 < 2*rapport3Inch)
                            pwm.avancer(rapport);
                        stopMinuterie();
                        resetMinuterie();

                        if(tempSirc == 0x03){
                            startMinuterie(duree);
                            while(TCNT1 < 4*rapport3Inch)
                                pwm.avancer(rapport);
                            stopMinuterie();
                            resetMinuterie();
                        } else if(tempSirc == 0x04){
                            startMinuterie(duree);
                            while(TCNT1 < 3*rapport3Inch)
                                pwm.avancer(rapport);
                            stopMinuterie();
                            resetMinuterie();
                        } else {
                            startMinuterie(duree);
                            while(TCNT1 < 2*rapport3Inch)
                                pwm.avancer(rapport);
                            stopMinuterie();
                            resetMinuterie();

                    break;
                    
                    case P7P8P9:
                        startMinuterie(duree);
                        while(TCNT1 < 2*rapport3Inch)
                            pwm.avancer(rapport);
                        stopMinuterie();
                        resetMinuterie();

                        if(tempSirc == 0x06){
                            startMinuterie(duree);
                            while(TCNT1 < 4*rapport3Inch)
                                pwm.avancer(rapport);
                            stopMinuterie();
                            resetMinuterie();
                        } else if(tempSirc == 0x07){
                            startMinuterie(duree);
                            while(TCNT1 < 3*rapport3Inch)
                                pwm.avancer(rapport);
                            stopMinuterie();
                            resetMinuterie();
                        } else {
                            startMinuterie(duree);
                            while(TCNT1 < 2*rapport3Inch)
                                pwm.avancer(rapport);
                            stopMinuterie();
                            resetMinuterie();
                    break;

                }

                stopMinuterie();
                resetMinuterie();
                etat_analyze = WAIT;

            break;

            case WAIT:
                tourner90Droite(rapport);
                pwm.arreter();
                _delay_ms(3000);
                //playNote(45, 3000);
                etat_analyze = GOTO_S3;
                
            break;

            case GOTO_S3:
                
                pwm.tourner90Droite(rapport);
                if(tempSirc == 0x02 ||
                tempSirc == 0x05 ||
                tempSirc == 0x08 ){
                    startMinuterie(duree);
                    while(TCNT1 < 2*rapport3Inch)
                        pwm.avancer(rapport);
                } else  if(tempSirc == 0x01 ||
                tempSirc == 0x04 ||
                tempSirc == 0x07 ) {
                    startMinuterie(duree);
                    while(TCNT1 < 3*rapport3Inch)
                        pwm.avancer(rapport);
                } else {
                    startMinuterie(duree);
                    while(TCNT1 < 4*rapport3Inch)
                        pwm.avancer(rapport);
                }
                stopMinuterie();
                resetMinuterie();
                pwm.tourner90Gauche(rapport);
                while(valueMap = 0x00){
                    pwm.avancer(rapport);
                }

                pwm.avancementAjuste(rapport, valueMap);
                

            break;


                
        }
        
    }
}

