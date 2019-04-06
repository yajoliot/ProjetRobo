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
    PIEZO_INIT(DDD4, DDD5, 50);
    

    DDRA = 0x00;
    DDRC = 0xFF;
    DDRB = 0xFF;

    uint8_t rapport = pwm.getVitesseDefault();
    uint8_t duree = 0xFF;

    enum etats {INIT, ANALYSE_IR}
    enum aStates {IR_WAIT, P1P2P3 ,P4P5P6, P7P8P9}
    uint32_t rapport3Inch;
    etats etat = INIT;
    aStates state = IR_WAIT;
    uint8_t valueMap;
    

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
                    rapport3Inch = (TCNT*7.62)/10.3;
                    etat = ANALYSE_IR;
                }

                stopMinuterie();
                resetMinuterie();

            break;

            case ANALYSE_IR:

                switch(state){

                    case IR_WAIT:
                    if(sirc_receiver.getCmd() == 0x00 ||
                    sirc_receiver.getCmd() == 0x01 ||
                    sirc_receiver.getCmd() == 0x02 ){
                        state = P1P2P3;
                    } else if (sirc_receiver.getCmd() == 0x03 ||
                    sirc_receiver.getCmd() == 0x04 ||
                    sirc_receiver.getCmd() == 0x05 ) {
                        state = P4P5P6;
                    } else {
                        state = P7P8P9;
                    }
                    

                    case P1P2P3:

                        startMinuterie(duree);
                        while(TCNT < 3*rapport3Inch)
                            pwm.avancer2*(rapport);
                        stopMinuterie();
                        resetMinuterie();
                        pwm.tourner90Gauche(rapport);
                        if(sirc_receiver.getCmd() == 0x00){
                            startMinuterie(duree);
                            while(TCNT < 4*rapport3Inch)
                                pwm.avancer2*(rapport);
                            stopMinuterie();
                            resetMinuterie();
                        } else if(sirc_receiver.getCmd() == 0x01){
                            startMinuterie(duree);
                            while(TCNT < 3*rapport3Inch)
                                pwm.avancer2*(rapport);
                            stopMinuterie();
                            resetMinuterie();
                        } else {
                            startMinuterie(duree);
                            while(TCNT < 2*rapport3Inch)
                                pwm.avancer2*(rapport);
                            stopMinuterie();
                            resetMinuterie();
                        }

                    break;

                    case P4P5P6:

                        while(TCNT < 2*rapport3Inch)
                            pwm.avancer(rapport);
                        stopMinuterie();
                        resetMinuterie();
                        if(sirc_receiver.getCmd() == 0x03){
                            startMinuterie(duree);
                            while(TCNT < 4*rapport3Inch)
                                pwm.avancer2*(rapport);
                            stopMinuterie();
                            resetMinuterie();
                        } else if(sirc_receiver.getCmd() == 0x04){
                            startMinuterie(duree);
                            while(TCNT < 3*rapport3Inch)
                                pwm.avancer2*(rapport);
                            stopMinuterie();
                            resetMinuterie();
                        } else {
                            startMinuterie(duree);
                            while(TCNT < 2*rapport3Inch)
                                pwm.avancer2*(rapport);
                            stopMinuterie();
                            resetMinuterie();

                    break;
                    
                    case P7P8P9:

                        while(TCNT < 2*rapport3Inch)
                            pwm.avancer(rapport);
                        stopMinuterie();
                        resetMinuterie();
                        if(sirc_receiver.getCmd() == 0x06){
                            startMinuterie(duree);
                            while(TCNT < 4*rapport3Inch)
                                pwm.avancer2*(rapport);
                            stopMinuterie();
                            resetMinuterie();
                        } else if(sirc_receiver.getCmd() == 0x07){
                            startMinuterie(duree);
                            while(TCNT < 3*rapport3Inch)
                                pwm.avancer2*(rapport);
                            stopMinuterie();
                            resetMinuterie();
                        } else {
                            startMinuterie(duree);
                            while(TCNT < 2*rapport3Inch)
                                pwm.avancer2*(rapport);
                            stopMinuterie();
                            resetMinuterie();
                    break;

                }

                stopMinuterie();
                resetMinuterie();
                state = WAIT;

            break;

            case WAIT:
                tourner90Droite(rapport);
                playNote(45, 3000);
                state = GOTO_S3;
                
            break;

            case GOTO_S3:
                
                tourner90Droite(rapport);
                if(sirc_receiver.getCmd() == 0x02 ||
                sirc_receiver.getCmd() == 0x05 ||
                sirc_receiver.getCmd() == 0x08 ){
                    startMinuterie(duree);
                    while(TCNT < 2*rapport3Inch)
                        avancer(rapport);
                } else  if(sirc_receiver.getCmd() == 0x01 ||
                sirc_receiver.getCmd() == 0x04 ||
                sirc_receiver.getCmd() == 0x07 ) {
                    startMinuterie(duree);
                    while(TCNT < 3*rapport3Inch)
                        avancer(rapport);
                } else {
                    startMinuterie(duree);
                    while(TCNT < 4*rapport3Inch)
                        avancer(rapport);
                }
                stopMinuterie();
                resetMinuterie();
                tourner90Gauche(rapport);
                while(valueMap = 0x00){
                    pwm.avancer(rapport);
                }

                pwm.avancementAjuste(rapport, valueMap);
                

            break;


                
        }
        
    }
}

