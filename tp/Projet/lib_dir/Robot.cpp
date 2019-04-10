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

    PWM pwm;
    LineTracker lineTracker;
    //PIEZO_INIT(DDD4, DDD5, 50); Le piezo Fuck avc TCCR1A de minuterie
    

    DDRC = 0xFF;
    DDRB = 0xFF;

    uint8_t rapport = pwm.getVitesseDefault();
    uint8_t duree = 0xFF;

    enum etats {INIT, ANALYSE_IR, WAIT, GOTO_S3, NEXT};
    enum aStates {IR_WAIT, P1P2P3 ,P4P5P6, P7P8P9};
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

                        pwm.avancerTimer(3, rapport3Inch);

                        if(tempSirc == 0x00){
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(4, rapport3Inch);
                        } else if(tempSirc == 0x01){
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(3, rapport3Inch);
                        } else {
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(2, rapport3Inch);
                        }  
                        etat = WAIT;
                    break;

                    case P4P5P6:

                        pwm.avancerTimer(2, rapport3Inch);

                        if(tempSirc == 0x03){
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(4, rapport3Inch);
                        } else if(tempSirc == 0x04){
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(3, rapport3Inch);
                        } else {
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(2, rapport3Inch);
                        }  
                        etat = WAIT;
                    break;
                    
                    case P7P8P9:

                        pwm.avancerTimer(1, rapport3Inch);

                        if(tempSirc == 0x06){
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(4, rapport3Inch);
                        } else if(tempSirc == 0x07){
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(3, rapport3Inch);
                        } else {
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(2, rapport3Inch);
                        }  
                        etat = WAIT;        
                    break;

                }


            break;

            case WAIT:
                pwm.tourner90Precis(1, rapport);
                _delay_ms(3000); //playNote(45, 3000);
                etat = GOTO_S3;
                
            break;

            case GOTO_S3:
                
                pwm.tourner90Droite(rapport);
                if( tempSirc == 0x02 ||
                    tempSirc == 0x05 ||
                    tempSirc == 0x08 ){

                    pwm.avancerTimer(2, rapport3Inch);

                } else  if( tempSirc == 0x01 ||
                            tempSirc == 0x04 ||
                            tempSirc == 0x07 ) {
                    pwm.avancerTimer(3, rapport3Inch);
                } else {
                    pwm.avancerTimer(4, rapport3Inch);
                }

                pwm.tourner90Precis(0, pwm.getVitesseDefault());
                pwm.arreter();
                _delay_ms(500);
                while(valueMap == 0x00){
                    pwm.avancer(rapport);
                    lineTracker.updateValueMap();
                    valueMap = lineTracker.getValueMap();
                }
                pwm.arreter();
                _delay_ms(3000);
                etat = NEXT;
                

            break;

            case NEXT:
                pwm.avancementAjuste(rapport, valueMap);
            break;


        }
                
        
        
    }
    
}



void Robot::RunCMD2(){
    PWM pwm;
    LineTracker lineTracker;
    enum etats {LIGNE_DROITE, TOURNE_GAUCHE, TOURNE_DROITE, PRE_BOITE, BOITE, POST_BOITE, ARRETE};
    int etat = LIGNE_DROITE;
    DDRC = 0xFF;
    DDRB = 0xFF;
    uint8_t rapport = pwm.getVitesseDefault(); // à changer pas bon -xavier
    bool boolBoite = false;
    bool tournerGauche = false;
    bool tournerDroite = false;
    pwm.avancer(pwm.getVitesseDefault());
    
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
    PWM pwm;
    LineTracker lineTracker;
    enum etats {LIGNE_DROITE, TOURNE_GAUCHE, TOURNE_DROITE, PRE_BOITE, BOITE, POST_BOITE, ARRETE};
    int etat = LIGNE_DROITE;
    DDRC = 0xFF;
    DDRB = 0xFF;
    uint8_t rapport = pwm.getVitesseDefault(); // à changer pas bon -xavier
    bool boolBoite = false;
    bool tournerGauche = false;
    bool tournerDroite = false;

    for(;;){
        
        lineTracker.updateValueMap();
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
        }
        
    }
    
}

void RunCMDCoin(){
    PWM pwm;
    LineTracker lineTracker;
    enum etats {INIT, TOURNANT_GAUCHE, DROIT, AJUSTEMENT, TOURNER, END};
    int etat = INIT;
    DDRC = 0xFF;
    DDRB = 0xFF;
    uint8_t rapport = pwm.getVitesseDefault(); // Ã  changer pas bon -xavier
    bool boolBoite = false;
    bool tournerGauche = false;
    bool tournerDroite = false;
    uint16_t duree = 0xFFFF;
    uint8_t valueMap;
    uint16_t temps;
    bool antiRebond = true;

    for(;;){
     
     lineTracker.updateValueMap();
     PORTC = lineTracker.getValueMap();
     valueMap = lineTracker.getValueMap();

     switch(etat){

        case INIT:
            DEBUG_INFO((uint8_t*) "INIT");
            pwm.avancementAjuste(rapport, valueMap);
            if(valueMap == 7 || valueMap == 3 || valueMap == 15)
                etat = TOURNANT_GAUCHE;
        break;

        case TOURNANT_GAUCHE:
            DEBUG_INFO((uint8_t*) "tournantGauche");
            pwm.avancer(pwm.getVitesseDefault());
            if(valueMap == 0)
                etat = DROIT;
        break;

        case DROIT :
            DEBUG_INFO((uint8_t*) "DROIT");
            startMinuterie(duree);
            pwm.avancer(pwm.getVitesseDefault());
            DEBUG_PARAMETER_VALUE((uint8_t*)"Minuterie", (void*)&TCNT1);
            if(valueMap != 0){
                etat = AJUSTEMENT;
                pwm.arreter();
                
                temps = TCNT1;
                stopMinuterie();
                resetMinuterie();
                etat = AJUSTEMENT;
            }

        break;

        case AJUSTEMENT:
            DEBUG_INFO((uint8_t*) "AJUSTEMENT");
            pwm.avancementAjuste(rapport, valueMap);
            startMinuterie(duree);
            if(TCNT1 >= ((temps) + (temps>>2))) {
                etat = TOURNER;
                stopMinuterie();
                resetMinuterie();
            }
            

        break;

        case TOURNER:
            DEBUG_INFO((uint8_t*) "TOURNER");
            pwm.tourner90Gauche(pwm.getVitesseDefault());
            if(antiRebond){
                _delay_ms(750); //antirebond pour ne pas lire le '4' de la petite ligne
                lineTracker.updateValueMap();
                valueMap = lineTracker.getValueMap();
                antiRebond = false;
            }
            
            if(valueMap == 4){
                pwm.tourner90Droite(pwm.getVitesseDefault());
                _delay_ms(250);
                pwm.arreter();
                etat = END;
            }
        break;

        case END:
            
        break;
                



     }

    }
}