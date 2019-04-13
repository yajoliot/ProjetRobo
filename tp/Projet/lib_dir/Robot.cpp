#include "Robot.h"
#include "debug.h"
    
volatile etats etat = INIT;
volatile bool boolISR = false;

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

Robot::~Robot(){
   
}

void Robot::Run(uint8_t IRCom){
    uint8_t valueMap = lineTracker->getValueMap();
    for(;nCMD != 5;){
        switch(IRCom){
            case 0x00:
                RunCMD1();
                nCMD++;
                if(nCMD == 5)
                    break;

            case 0x01:
                RunCMD2();

                nCMD++;
                if(nCMD == 5)
                    break;
            DEBUG_PARAMETER_VALUE((uint8_t*)"avant", &valueMap);
            case 0x02:
                DEBUG_PARAMETER_VALUE((uint8_t*)"runcmd3avant", &valueMap);
                RunCMD3();
                DEBUG_PARAMETER_VALUE((uint8_t*)"runcmd3 apres", &valueMap);
                nCMD++;
                if(nCMD == 5)
                    break;

            case 0x03:
                RunCMD4();
                nCMD++;
                if(nCMD == 5)
                    break;
            case 0x04:
                RunCMDCoin();
                nCMD++;
                if(nCMD == 5)
                    break;
        }
    } 
    abort();
}

void Robot::RunCMD1(){

    //PIEZO_INIT(DDD4, DDD5, 50); Le piezo Fuck avc TCCR1A de minuterie
    

    DDRC = 0xFF;
    DDRB = 0xFF;

    uint8_t rapport = pwm->getVitesseDefault();
    uint8_t duree = 0xFF;

    enum states {INIT, ANALYSE_IR, WAIT, GOTO_S3, NEXT};
    enum aStates {IR_WAIT, P1P2P3 ,P4P5P6, P7P8P9};
    uint32_t rapport3Inch;
    states etat = INIT;
    aStates etat_analyze = IR_WAIT;
    uint8_t valueMap;
    bool loop = true;

    //tempSirc temp replacemtn for ir receiver
    uint8_t tempSirc = 0x00;
    

    for(;loop;){
    
        lineTracker->updateValueMap();
        uint8_t valueMap = lineTracker->getValueMap();
        PORTC = valueMap;

        switch(etat){
            case INIT:
                pwm->avancementAjuste(rapport, valueMap);
                startMinuterie(duree);
                if(valueMap == 0x1F){
                    rapport3Inch = (TCNT1 >> 1) + (TCNT1 >> 4) + (TCNT1 >> 5);
                    etat = ANALYSE_IR;
                    stopMinuterie();
                    resetMinuterie();
                }

            break;

            case ANALYSE_IR:

                switch(etat_analyze){

                    case IR_WAIT:
                        if( tempSirc == 0x00 ||
                            tempSirc == 0x01 ||
                            tempSirc == 0x02 ){
                            
                            etat_analyze = P1P2P3;

                        } else if ( tempSirc == 0x03 ||
                                    tempSirc == 0x04 ||
                                    tempSirc == 0x05 ) {

                            etat_analyze = P4P5P6;

                        } else {
                            etat_analyze = P7P8P9;
                        }    
                    break;

                    case P1P2P3:
                        
                        pwm->avancerTimer(3, rapport3Inch);

                        if(tempSirc == 0x00){
                            pwm->tourner90Precis(0, rapport);
                            pwm->avancerTimer(4, rapport3Inch);
                        } else if(tempSirc == 0x01){
                            pwm->tourner90Precis(0, rapport);
                            pwm->avancerTimer(3, rapport3Inch);
                        } else {
                            pwm->tourner90Precis(0, rapport);
                            pwm->avancerTimer(2, rapport3Inch);
                        }  
                        etat = WAIT;
                    break;

                    case P4P5P6:

                        pwm->avancerTimer(2, rapport3Inch);

                        if(tempSirc == 0x03){
                            pwm->tourner90Precis(0, rapport);
                            pwm->avancerTimer(4, rapport3Inch);
                        } else if(tempSirc == 0x04){
                            pwm->tourner90Precis(0, rapport);
                            pwm->avancerTimer(3, rapport3Inch);
                        } else {
                            pwm->tourner90Precis(0, rapport);
                            pwm->avancerTimer(2, rapport3Inch);
                        }  
                        etat = WAIT;
                    break;
                    
                    case P7P8P9:

                        pwm->avancerTimer(1, rapport3Inch);

                        if(tempSirc == 0x06){
                            pwm->tourner90Precis(0, rapport);
                            pwm->avancerTimer(4, rapport3Inch);
                        } else if(tempSirc == 0x07){
                            pwm->tourner90Precis(0, rapport);
                            pwm->avancerTimer(3, rapport3Inch);
                        } else {
                            pwm->tourner90Precis(0, rapport);
                            pwm->avancerTimer(2, rapport3Inch);
                        }  
                        etat = WAIT;        
                    break;

                }


            break;

            case WAIT:
                pwm->tourner90Precis(1, rapport);
                _delay_ms(3000); //playNote(45, 3000);
                etat = GOTO_S3;
                
            break;

            case GOTO_S3:
                
                pwm->tourner90Droite(rapport);
                if( tempSirc == 0x02 ||
                    tempSirc == 0x05 ||
                    tempSirc == 0x08 ){

                    pwm->avancerTimer(2, rapport3Inch);

                } else  if( tempSirc == 0x01 ||
                            tempSirc == 0x04 ||
                            tempSirc == 0x07 ) {
                    pwm->avancerTimer(3, rapport3Inch);
                } else {
                    pwm->avancerTimer(4, rapport3Inch);
                }

                pwm->tourner90Precis(0, pwm->getVitesseDefault());
                pwm->arreter();
                _delay_ms(500);
                while(valueMap == 0x00){
                    pwm->avancer(rapport);
                    lineTracker->updateValueMap();
                    valueMap = lineTracker->getValueMap();
                }
                pwm->arreter();
                _delay_ms(3000);
                etat = NEXT;
                

            break;

            case NEXT:

                loop = false;
                
            break;


        }
                
        
        
    }
    
}



void Robot::RunCMD2(){
    
    enum states {INIT, TOURNE_DROITE, COIN, TOURNE_GAUCHE, NEXT};
    int etat = INIT;
    DDRC = 0xFF;
    DDRB = 0xFF;
    uint8_t rapport = pwm->getVitesseDefault(); // à changer pas bon -xavie
    bool loop = true;

    for(;loop;){
        
        lineTracker->updateValueMap();
        uint8_t valueMap = lineTracker->getValueMap();
        

        //TODO:  mettre dans une fonction
        
        //TODO: AJOUTER LE CAS VALUEMAP == 0
        
        //Conditions pour entrer dans la boite
        
        //TODO:  mettre dans une fonction
        switch(etat){
            case INIT:
                pwm->ralentissementGauche(rapport, valueMap);
                if(valueMap == 24 || valueMap == 28 || valueMap == 30 || valueMap == 31){
                    etat = TOURNE_DROITE;
                }
            break;
            
            case TOURNE_DROITE:
                //TODO:  mettre dans une fonction
                while(valueMap != 4){
                    lineTracker->updateValueMap();
                    valueMap = lineTracker->getValueMap();
                    pwm->tournantDroite(rapport, valueMap);
                }
                etat = COIN;
            break;

            case COIN:
                    //TODO:  mettre dans une fonction
                pwm->ralentissementGauche(rapport, valueMap);
                if(valueMap == 3 || valueMap == 7 || valueMap == 15 || valueMap == 31)
                    etat = TOURNE_GAUCHE;
            break;

            case TOURNE_GAUCHE:
                while(valueMap != 4){
                    lineTracker->updateValueMap();
                    valueMap = lineTracker->getValueMap();
                    pwm->tournantGauche(rapport, valueMap);
                }
                etat = NEXT;
            break;

            case NEXT:
                loop = false;
                DEBUG_PARAMETER_VALUE((uint8_t*)"NEXT", &valueMap);
            break;

        }       
        
    }
}



void Robot::RunCMD3(){

    
    enum etats2 {INIT2,ANTI_REBOND, DIST_1, DIST_2};

    volatile etats2 etat2 = INIT2;

    DDRC = 0xFF;
    DDRB = 0xFF;
    isr_INIT();
    uint16_t duree  = 0xFFFF;
    uint16_t distTime1 = 0;
    uint16_t distTime2 = 0;
    uint16_t compare_value;
    bool loop = true;
    bool droite = true;
    bool analyse = false;
    uint8_t valueMap = lineTracker->getValueMap();
    uint8_t rapport;

    DEBUG_PARAMETER_VALUE((uint8_t*)"variables", (void*) &loop);

    for(;loop;){
        
        DEBUG_PARAMETER_VALUE((uint8_t*)"LIMbES", &valueMap);

        DEBUG_PARAMETER_VALUE((uint8_t*)"", (void*) &etat);
        lineTracker->updateValueMap();
        
        

        if(lineTracker->getValueMap() == 0x1F && !analyse){
            DEBUG_PARAMETER_VALUE((uint8_t*)"LIMBES2", &valueMap);
            etat = INTWAIT;
            analyse = true;
        }
        

        switch(etat){
            case INIT:
                DEBUG_PARAMETER_VALUE((uint8_t*)"INIT", &valueMap);
                pwm->avancementAjuste(rapport, lineTracker->getValueMap());
                break;

            case INTWAIT:
                pwm->arreter();
                rapport = pwm->getVitesseDefault();
                break;

            case ANALYSE:
                if(lineTracker->getValueMap() == 0x1F){
                    etat2 = ANTI_REBOND;
                }
                    
                switch(etat2){
                    case INIT2:
                        pwm->avancer(rapport);
                    break;

                    case ANTI_REBOND:

                        pwm->avancer(rapport);

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
                                lineTracker->getValueMap() == 0x18 ||
                                lineTracker->getValueMap() == 0x01 ||
                                lineTracker->getValueMap() == 0x10 ||
                                lineTracker->getValueMap() == 0x0F ||
                                lineTracker->getValueMap() == 0x1E
                                 )){

                            lineTracker->updateValueMap();
                            pwm->avancer(rapport);
                        }
                        if(lineTracker->getValueMap() == 0x01 || lineTracker->getValueMap() == 0x03 ||lineTracker->getValueMap() == 0x07 || lineTracker->getValueMap() == 0x0F ) //if found left
                            droite = false;

                        distTime1 = TCNT1;
                        etat2 = DIST_2;
                    break;

                    case DIST_2:
                        startMinuterie(duree);
                        if(droite){
                            while(!( 
                                lineTracker->getValueMap() == 0x07 || 
                                lineTracker->getValueMap() == 0x03 ||  
                                lineTracker->getValueMap() == 0x01 ||
                                lineTracker->getValueMap() == 0x0F)){
                                lineTracker->updateValueMap();
                                pwm->avancer(rapport);
                            }

                        } else {
                            while(!(lineTracker->getValueMap() == 0x1C || 
                                lineTracker->getValueMap() == 0x18 ||
                                lineTracker->getValueMap() == 0x10 ||
                                lineTracker->getValueMap() == 0x1E
                                 )){
                                lineTracker->updateValueMap();
                                pwm->avancer(rapport);
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
                while(boolISR){
                    if( abs(distTime2 - distTime1) < compare_value + compare_value && droite){ 
                        PORTC = 0x08;
                    } else if ( abs(distTime2 - distTime1) > compare_value + compare_value && droite) {
                        PORTC = 0x04;
                    } else if( (abs(distTime2 - distTime1)) < compare_value + compare_value && !droite  ) {
                        PORTC = 0x02;
                    } else {
                        PORTC = 0x01;
                    } 
                }
                
                boolISR = true;
                etat = NEXT;
        
            break;
            case NEXT:
                loop = false;
                DEBUG_PARAMETER_VALUE((uint8_t*)"apres", &valueMap);
            break;
        }
        
    } 
    DEBUG_PARAMETER_VALUE((uint8_t*)"apres", &valueMap);
}

void Robot::RunCMD4(){
    enum states {LIGNE_DROITE, TOURNE_GAUCHE, TOURNE_DROITE, PRE_BOITE, BOITE, POST_BOITE, PRE_NEXT, NEXT};
    int etat = LIGNE_DROITE;
    DDRC = 0xFF;
    DDRB = 0xFF;
    uint8_t rapport = pwm->getVitesseDefault(); // à changer pas bon -xavier
    bool boolBoite = false;
    bool tournerGauche = false;
    bool tournerDroite = false;
    uint8_t nbBoites = 0;
    bool loop = true;

    for(;;){
        
        lineTracker->updateValueMap();
        uint8_t valueMap = lineTracker->getValueMap();
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
                    pwm->avancementAjuste(rapport, valueMap);
                break;
            
            case TOURNE_GAUCHE:
                    //TODO:  mettre dans une fonction
                    pwm->tournantGauche(rapport, valueMap);
                break;

            case TOURNE_DROITE:
                    //TODO:  mettre dans une fonction
                    pwm->tournantDroite(rapport, valueMap);
                break;

            case PRE_BOITE:
                    //TODO: à mettre dans fonction à part
                
                    while(lineTracker->getValueMap() == 31){
                        pwm->avancer(pwm->getVitesseDefault());
                        lineTracker->updateValueMap();
                    }
                    boolBoite = true;
                    etat = BOITE;
                break;

            case BOITE : 
                    //TODO: à mettre dans fonction à part
                    while( !(lineTracker->getValueMap() == 31) ){
                        lineTracker->updateValueMap();
                        valueMap = lineTracker->getValueMap();
                        pwm->boite(rapport, valueMap);
                        lineTracker->updateValueMap();
                    }
                    etat = POST_BOITE;
                    
                break;

            case POST_BOITE:
                    nbBoites++;
                    while(lineTracker->getValueMap() == 31){
                        pwm->avancer(pwm->getVitesseDefault());
                        lineTracker->updateValueMap();
                    }
                    boolBoite = false;
                    if(nbBoites >= 3)
                        etat = NEXT;
                    else
                        etat = LIGNE_DROITE;
                break;
            
            case NEXT:
                    loop = false;
                break;
        }
        
    }
    
}

void Robot::RunCMDCoin(){
    enum etats {INIT, TOURNANT_GAUCHE, DROIT, AJUSTEMENT, TOURNER, NEXT};
    int etat = INIT;
    DDRC = 0xFF;
    DDRB = 0xFF;
    uint8_t rapport = pwm->getVitesseDefault(); // Ã  changer pas bon -xavier
    bool boolBoite = false;
    bool tournerGauche = false;
    bool tournerDroite = false;
    uint16_t duree = 0xFFFF;
    uint8_t valueMap;
    uint16_t temps;
    bool antiRebond = true;
    bool loop = true;

    for(;loop;){
     
     lineTracker->updateValueMap();
     PORTC = lineTracker->getValueMap();
     valueMap = lineTracker->getValueMap();

     switch(etat){

        case INIT:
            DEBUG_INFO((uint8_t*) "INIT");
            pwm->avancementAjuste(rapport, valueMap);
            if(valueMap == 7 || valueMap == 3 || valueMap == 15)
                etat = TOURNANT_GAUCHE;
        break;

        case TOURNANT_GAUCHE:
            pwm->arreter();
            _delay_ms(100);
            DEBUG_INFO((uint8_t*) "tournantGauche");
            pwm->avancer(pwm->getVitesseDefault());
            if(valueMap == 0)
                etat = DROIT;
        break;

        case DROIT :
            DEBUG_INFO((uint8_t*) "DROIT");
            startMinuterie(duree);
            pwm->avancer(pwm->getVitesseDefault());
            DEBUG_PARAMETER_VALUE((uint8_t*)"Minuterie", (void*)&TCNT1);
            if(valueMap != 0){
                etat = AJUSTEMENT;
                pwm->arreter();
                
                temps = TCNT1;
                stopMinuterie();
                resetMinuterie();
                etat = AJUSTEMENT;
            }

        break;

        case AJUSTEMENT:
            DEBUG_INFO((uint8_t*) "AJUSTEMENT");
            pwm->avancer(rapport);
            startMinuterie(duree);
            if(TCNT1 >= (temps) - (temps >> 2)) {
                etat = TOURNER;
                stopMinuterie();
                resetMinuterie();
            }
            

        break;

        case TOURNER:
            DEBUG_INFO((uint8_t*) "TOURNER");
            pwm->tourner90Precis(0,pwm->getVitesseDefault());
            if(antiRebond){
                _delay_ms(300); //antirebond pour ne pas lire le '4' de la petite ligne
                lineTracker->updateValueMap();
                valueMap = lineTracker->getValueMap();
                antiRebond = false;
            }
            
            if(valueMap == 4){
                pwm->rotateDroite(pwm->getVitesseDefault());
                _delay_ms(250);
                pwm->arreter();
                etat = NEXT;
            }
        break;

        case NEXT:
                loop = false;
        break;
                

     }

    }
}

void Robot::test(){


    
}
