#include "Robot.h"
#include "debug.h"
    
volatile etats etat = INIT;

 void isr_INIT() {

    DDRD = 0x00;
    DDRB = 0xff;

    cli ();

    EIMSK |= (1 << INT0);

    EICRA |= (1 << ISC01) | (1 << ISC00); // EICRA = EICRA | (1 << ISC01)

    sei();
}

void * operator new(size_t size)
{
  return malloc(size);
}


Robot::Robot(){
    pwm = new PWM();
    lineTracker = new LineTracker();
}

void Robot::Run(uint8_t IRCom){
    switch(IRCom){
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

    enum etats2 {INIT2,ANTI_REBOND, DIST_1, DIST_2};

    volatile etats2 etat2 = INIT2;

    DDRC = 0xFF;
    DDRB = 0xFF;
    isr_INIT();
    uint8_t rapport = 150;
    uint16_t duree  = 0xFFFF;
    uint16_t distTime1 = 0;
    uint16_t distTime2 = 0;
    uint16_t compare_value;
    bool droite = true;
    bool analyse = false;

    for(;;){

        lineTracker->updateValueMap();
        if(lineTracker->getValueMap() == 0x1F && !analyse){
            etat = INTWAIT;
            analyse = true;
        }
        

        switch(etat){
            case INIT:
                pwm->avancementAjuste(rapport, lineTracker->getValueMap());
                break;

            case INTWAIT:
                pwm->arreter();
                break;

            case ANALYSE:
                if(lineTracker->getValueMap() == 0x1F){
                    etat2 = ANTI_REBOND;
                }
                    
                switch(etat2){
                    case INIT2:
                        pwm->avancementAjuste(rapport, lineTracker->getValueMap());
                    break;

                    case ANTI_REBOND:

                        pwm->avancementAjuste(rapport, lineTracker->getValueMap());

                        if(lineTracker->getValueMap() == 0x04 || 
                           lineTracker->getValueMap() == 0x06 || 
                           lineTracker->getValueMap() == 0x0C){
                            
                            etat2 = DIST_1;

                        }

                        _delay_ms(1);
                    break;

                    case DIST_1:
                        startMinuterie(duree);
                        while(!(lineTracker->getValueMap() == 0x1C || 
                                lineTracker->getValueMap() == 0x07 || 
                                lineTracker->getValueMap() == 0x03 ||  
                                lineTracker->getValueMap() == 0x18)){

                            lineTracker->updateValueMap();
                            pwm->avancementAjuste(rapport, lineTracker->getValueMap());
                        }
                        if(lineTracker->getValueMap() == 0x07 || lineTracker->getValueMap() == 0x06) //if found left
                            droite = false;

                        distTime1 = TCNT1;
                        etat2 = DIST_2;
                    break;

                    case DIST_2:
                        startMinuterie(duree);
                        if(droite){
                            while(!(lineTracker->getValueMap() == 0x07 || lineTracker->getValueMap() == 0x06)){
                                lineTracker->updateValueMap();
                                pwm->avancementAjuste(rapport, lineTracker->getValueMap());
                            }

                        } else {
                            while(!(lineTracker->getValueMap() == 0x1C || lineTracker->getValueMap() == 0x18)){
                                lineTracker->updateValueMap();
                                pwm->avancementAjuste(rapport, lineTracker->getValueMap());
                            }
                        }

                        distTime2 = TCNT1;
                        etat = WAIT_TILL_END;
                        
                    break;
                    
                }
                
                
            break;

            case WAIT_TILL_END:
                if(lineTracker->getValueMap() == 0x00){
                    etat = END;
                    pwm->arreter();
                }
                compare_value = distTime1;
            break;

            case END:
                pwm->arreter();

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
    enum etats {LIGNE_DROITE, TOURNE_GAUCHE, TOURNE_DROITE, PRE_BOITE, BOITE, POST_BOITE, ARRETE};
    int etat = LIGNE_DROITE;
    DDRC = 0xFF;
    DDRB = 0xFF;
    uint8_t rapport = pwm->getVitesseDefault();
    bool boolBoite = false;
    bool tournerGauche = false;
    bool tournerDroite = false;
    uint8_t valueMap;

    for(;;){
        DEBUG_PARAMETER_VALUE((uint8_t*)"état", &etat);

        lineTracker->updateValueMap();
        valueMap = lineTracker->getValueMap();
        PORTC = valueMap;
        


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
        

        switch(etat){
            case LIGNE_DROITE:
                    pwm->avancementAjuste(rapport, valueMap);
                break;
            
            case TOURNE_GAUCHE:

                    pwm->tournantGauche(rapport, valueMap);
                break;

            case TOURNE_DROITE:
                    pwm->tournantDroite(rapport, valueMap);
                break;

            case PRE_BOITE:
                    while(lineTracker->getValueMap() == 31){
                        pwm->avancer(pwm->getVitesseDefault());
                        lineTracker->updateValueMap();
                    }
                    boolBoite = true;
                    etat = BOITE;
                break;

            case BOITE : 
                    while( !(lineTracker->getValueMap() == 31) ){
                        pwm->boite(rapport, pwm->getVitesseDefault());
                        lineTracker->updateValueMap();
                    }
                    etat = POST_BOITE;
                    
                break;

            case POST_BOITE:
                    while(lineTracker->getValueMap() == 31){
                        pwm->avancer(pwm->getVitesseDefault());
                        lineTracker->updateValueMap();
                    }
                    boolBoite = false;
                    etat = LIGNE_DROITE;
                break;
            
            case ARRETE:
                    pwm->arreter();
                break;
        }
        
    }
}