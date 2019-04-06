#include "Robot.h"

Robot::Robot(){
    
}

void Robot::Run(uInt8_t IRCOM){
    switch(IRCOM){
        case 0x00:
            RunCMD1();
        break;

        case 0x01:
            RunCMD2();
        break;

        case 0x02:
            RunCMD3();
        break;

        case 0x03:
            RunCMD4();
        break;
    }
}

void Robot::RunCMD1(){
    
}

void Robot::RunCMD2(){
    
}

void Robot::RunCMD3(){

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
                        _delay_ms(1);
                    break;

                    case DIST_1:
                        startMinuterie(duree);
                        while(!(lineTracker.getValueMap() == 0x1C
                            || lineTracker.getValueMap() == 0x07 || lineTracker.getValueMap() == 0x03 || lineTracker.getValueMap() == 0x18)){
                            lineTracker.updateValueMap();
                            PORTC = lineTracker.getValueMap();
                            pwm.avancementAjuste(rapport, lineTracker.getValueMap());
                        }
                        if(lineTracker.getValueMap() == 0x07 || lineTracker.getValueMap() == 0x06) //if found left
                            droite = false;

                        distTime1 = TCNT1;
                        etat2 = DIST_2;
                    break;

                    case DIST_2:
                        startMinuterie(duree);
                        if(droite){
                            while(!(lineTracker.getValueMap() == 0x07 || lineTracker.getValueMap() == 0x06)){
                                lineTracker.updateValueMap();
                                pwm.avancementAjuste(rapport, lineTracker.getValueMap());
                            }

                        } else {
                            while(!(lineTracker.getValueMap() == 0x1C || lineTracker.getValueMap() == 0x18)){
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
                compare_value = distTime1;
            break;

            case END:
                pwm.arreter();

                if( abs(distTime2 - distTime1) < compare_value  && droite){ 
                    PORTC = 0x10;
                } else if ( abs(distTime2 - distTime1) > compare_value && droite) {
                    PORTC = 0x08;
                } else if( (abs(distTime2 - distTime1) < compare_value) && !droite  ) {
                    PORTC = 0x04;
                } else {
                    PORTC = 0x02;
                } 
                
        
            break;


        }

        
        
    } 
}

void Robot::RunCMD4(){
    
}
