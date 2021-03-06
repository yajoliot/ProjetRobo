#include "Robot.h"
#include "debug.h"

    
volatile extern bool intWaitISR = false;
volatile extern bool boolISR = false;
volatile extern uint8_t pointCounterISR = 0;
volatile extern uint8_t cornerCounterISR = 0;
volatile extern bool useCornerISR = false;
volatile extern bool usePointISR = false;

void Robot::isr_INIT() {

    cli ();

    EIMSK |= (1 << INT0);

    EICRA |= (1 << ISC01) | (1 << ISC00); // EICRA = EICRA | (1 << ISC01)

    sei();

}



Robot::Robot(){}



uint8_t Robot::receive(){
    uint8_t result = 0;
    
    startMinuterie(0xFFFF);
    while(TCNT1 < 0x9FFF){
        if(false){ //verifyHeader()
            
        
            result = readBits(COMMAND_SIZE);

        }
    }
    stopMinuterie();
    resetMinuterie();

    //DEBUG_PARAMETER_VALUE((uint8_t*)"after while",(void*) &cornerCounterISR);
    if(useCornerISR){
        pointCounterISR = 0;
        usePointISR = false;
        boolISR = false;
        intWaitISR = false;
        return cornerCounterISR - 1;
    } else {
        return result - 1;
    }
}

void Robot::Run(uint8_t IRCom){
    for(;nCMD != 5;){
        switch(IRCom){ //OMG IMPORTANT CHANGER
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
            
            case 0x02:
                DEBUG_INFO((uint8_t*)"in case 02");
                RunCMD3();
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
                IRCom = 0x00;
                if(nCMD == 5)
                    break;
        }
    } 
    abort();
}

void Robot::RunCMD1(){

    

    uint8_t rapport = pwm.getVitesseDefault();
    uint8_t duree = 0xFF;

    enum states {INIT, ANALYSE_IR, WAIT, GOTO_S3,TOURNANT_GAUCHE , NEXT};
    enum aStates {IR_WAIT, P1P2P3 ,P4P5P6, P7P8P9};
    uint32_t rapport3Inch = 0;
    states etat = INIT;
    aStates etat_analyze = IR_WAIT;
    uint8_t valueMap;
    bool loop = true;

    uint8_t pointIR = 0x00;
    

    for(;loop;){
    
        lineTracker.updateValueMap();
        valueMap = lineTracker.getValueMap();
        PORTC = valueMap;
        rapport = pwm.getVitesseDefault();
        switch(etat){
            case INIT:
                
                startMinuterie(duree);
                while(valueMap != 0x1F){
                    lineTracker.updateValueMap();
                    valueMap = lineTracker.getValueMap();
                    pwm.avancementAjuste(rapport, valueMap);
                    rapport3Inch = (TCNT1 >> 1) + (TCNT1 >> 2);
                    etat = ANALYSE_IR;
                }
                stopMinuterie();
                resetMinuterie();

            break;

            case ANALYSE_IR:

                switch(etat_analyze){

                    case IR_WAIT:
                        startMinuterie(0xFFFF);
                        while(TCNT1 < 0x9FFF){
                            pwm.arreter();
                            if(false){ //verifyHeader()
                                pointIR = readBits(COMMAND_SIZE);
                            }
                        }
                        stopMinuterie();
                        resetMinuterie();


                        if (usePointISR == true){
                            boolISR = false;
                            intWaitISR = false;
                            pointIR = pointCounterISR - 1;

                        } else {
                            pointIR--;
                        }

                        if( pointIR == 0x00 ||
                            pointIR == 0x01 ||
                            pointIR == 0x02 ){
                            
                            etat_analyze = P1P2P3;

                        } else if ( pointIR == 0x03 ||
                                    pointIR == 0x04 ||
                                    pointIR == 0x05 ) {

                            etat_analyze = P4P5P6;

                        } else {
                            etat_analyze = P7P8P9;
                        }
                    break;

                    case P1P2P3:
                        
                        pwm.avancerTimer(3, rapport3Inch);

                        if(pointIR == 0x00){
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(3, rapport3Inch);
                        } else if(pointIR == 0x01){
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(2, rapport3Inch);
                        } else {
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(1, rapport3Inch);
                        }  
                        etat = WAIT;
                    break;

                    case P4P5P6:

                        pwm.avancerTimer(2, rapport3Inch);

                        if(pointIR == 0x03){
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(3, rapport3Inch);
                        } else if(pointIR == 0x04){
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(2, rapport3Inch);
                        } else {
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(1, rapport3Inch);
                        }  
                        etat = WAIT;
                    break;
                    
                    case P7P8P9:

                        pwm.avancerTimer(1, rapport3Inch);

                        if(pointIR == 0x06){
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(3, rapport3Inch);
                        } else if(pointIR == 0x07){
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(2, rapport3Inch);
                        } else {
                            pwm.tourner90Precis(0, rapport);
                            pwm.avancerTimer(1, rapport3Inch);
                        }  
                        etat = WAIT;        
                    break;

                }


            break;

            case WAIT:

                pwm.tourner90Precis(1, rapport);
                PIEZO_INIT(DDD5, DDD7, 50);
                PLAY_NOTE(45);
                _delay_ms(3000);
                setVolume(0);
                pwm.tourner90Precis(1, rapport);
                setVolume(50);
                _delay_ms(3000);
                setVolume(0);
                resetRegisters();
                etat = GOTO_S3;
                
            break;

            case GOTO_S3:
                
                if( pointIR == 0x02 ||
                    pointIR == 0x05 ||
                    pointIR == 0x08 ){

                    pwm.avancerTimer(1, rapport3Inch);

                } else  if( pointIR == 0x01 ||
                            pointIR == 0x04 ||
                            pointIR == 0x07 ) {
                    pwm.avancerTimer(2, rapport3Inch);
                } else {
                    pwm.avancerTimer(3, rapport3Inch);
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
                _delay_ms(500);
                etat = TOURNANT_GAUCHE;
                

            break;
            case TOURNANT_GAUCHE:

            while(valueMap != 0){
                lineTracker.updateValueMap();
                valueMap = lineTracker.getValueMap();
                pwm.avancementAjuste(rapport, valueMap);
            }
            
            while(valueMap != 4){
                lineTracker.updateValueMap();
                valueMap = lineTracker.getValueMap();
                pwm.tournantGauche(rapport, valueMap);
            }
            etat = NEXT;

            case NEXT:

                loop = false;
                
            break;


        }
                
        
        
    }
    
}



void Robot::RunCMD2(){
    
    
    enum states {INIT, TOURNE_DROITE, COIN, TOURNE_GAUCHE, NEXT};
    int etat = INIT;
    uint8_t rapport = pwm.getVitesseDefault(); // à changer pas bon -xavie
    bool loop = true;
    uint8_t valueMap;

    for(;loop;){
        
        lineTracker.updateValueMap();
        valueMap = lineTracker.getValueMap();
    
        switch(etat){
            case INIT:
                pwm.ralentissementGauche(rapport, valueMap);
                if(valueMap == 24 || valueMap == 28 || valueMap == 30 || valueMap == 31){
                    etat = TOURNE_DROITE;
                }
            break;
            
            case TOURNE_DROITE:

                pwm.tournantDroite(rapport, valueMap);
                if(valueMap == 4)
                etat = COIN;
            break;

            case COIN:
                pwm.ralentissementGauche(rapport, valueMap);
                if(valueMap == 0)
                    etat = TOURNE_GAUCHE;
            break;

            case TOURNE_GAUCHE:
                pwm.tournantGauche(rapport, valueMap);
                if (valueMap == 4)
                    etat = NEXT;
            break;

            case NEXT:
                loop = false;
                DEBUG_INFO((uint8_t*)"in case next");
            break;

        }       
        
    }
}



void Robot::RunCMD3(){

    enum states {INIT, INTWAIT, ANALYSE, WAIT_TILL_END, END, NEXT};
    enum etats2 {INIT2,ANTI_REBOND, DIST_1, DIST_2};
    states etat = INIT;
    etats2 etat2 = INIT2;
    uint16_t duree  = 0xFFFF;
    uint16_t distTime1 = 0;
    uint16_t distTime2 = 0;
    uint16_t compare_value = 0;
    bool loop = true;
    bool droite = true;
    bool analyse = false;
    uint8_t valueMap = lineTracker.getValueMap();
    uint8_t rapport;
    isr_INIT();
    boolISR = false;
    pointCounterISR = 0;
    usePointISR = 0;



    for(;loop;){
        
        lineTracker.updateValueMap();
        valueMap = lineTracker.getValueMap();



        switch(etat){
            case INIT:
                DEBUG_INFO((uint8_t*)"in case INIT");
                pwm.avancementAjuste(rapport, valueMap);
                if(valueMap == 0x1F ){
                    etat = INTWAIT;
                }
                break;

            case INTWAIT:
                DEBUG_INFO((uint8_t*)"in case intwait");
                pwm.arreter();
                rapport = pwm.getVitesseDefault();
                if(intWaitISR)
                    etat = ANALYSE;
                    pointCounterISR = 0;
                    cornerCounterISR = 0;
                    useCornerISR = false;
                    pointCounterISR = false;
                break;

            case ANALYSE:
                DEBUG_INFO((uint8_t*)"in case analyse");
                boolISR = false;
                usePointISR = false;
                pointCounterISR = 0;
                
                    
                switch(etat2){
                    case INIT2:
                        pwm.avancer(rapport);
                        if(valueMap == 0x1F){
                            etat2 = ANTI_REBOND;
                        }
                    break;

                    case ANTI_REBOND:

                        pwm.avancer(rapport);

                        if(valueMap == 0x04 || 
                           valueMap == 0x06 || 
                           valueMap == 0x0C){
                            
                            etat2 = DIST_1;

                        }

                    break;

                    case DIST_1:
                        startMinuterie(duree);
                        while(!(valueMap == 0x1C || 
                                valueMap == 0x07 || 
                                valueMap == 0x03 ||  
                                valueMap == 0x18 ||
                                valueMap == 0x01 ||
                                valueMap == 0x10 ||
                                valueMap == 0x0F ||
                                valueMap == 0x1E
                                )){
                            lineTracker.updateValueMap();
                            valueMap = lineTracker.getValueMap();
                            pwm.avancer(rapport);

                        }
                        lineTracker.updateValueMap();
                        valueMap = lineTracker.getValueMap();

                        if( valueMap == 0x01 || 
                            valueMap == 0x03 ||
                            valueMap == 0x07 || 
                            valueMap == 0x0F ) //if found left
                            droite = false;

                        distTime1 = TCNT1;
                        etat2 = DIST_2;
                    break;

                    case DIST_2:
                        startMinuterie(duree);
                        if(droite){
                            while(!( valueMap == 0x07 || 
                                     valueMap == 0x03 ||  
                                     valueMap == 0x01 ||
                                     valueMap == 0x0F)){
                                lineTracker.updateValueMap();
                                valueMap = lineTracker.getValueMap();
                                pwm.avancer(rapport);
                            }

                        } else {
                            while(!(valueMap == 0x1C || 
                                    valueMap == 0x18 ||
                                    valueMap == 0x10 ||
                                    valueMap == 0x1E )){
                                lineTracker.updateValueMap();
                                valueMap = lineTracker.getValueMap();
                                pwm.avancer(rapport);
                            }
                        }
                        boolISR = false;
                        usePointISR = false;
                        pointCounterISR = 0;
                        distTime2 = TCNT1;
                        etat = WAIT_TILL_END;
                        
                    break;
                    
                }
                
                
            break;

            case WAIT_TILL_END:
                if(valueMap == 0x00){
                    etat = END;
                    pwm.arreter();
                }
                compare_value = distTime1;
            break;

            case END:
                pwm.arreter();
                while(!boolISR){
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
                
                boolISR = false;
                etat = NEXT;
                pointCounterISR = 0;
                usePointISR = false;
                intWaitISR = false;
        
            break;
            case NEXT:
                loop = false;
            break;
        }
        
    } 
}

void Robot::RunCMD4(){
    enum states {INIT, TOURNE_GAUCHE, LIGNE_DROITE,  PRE_BOITE, BOITE, POST_BOITE, NEXT};
    int etat = INIT;
    uint8_t rapport = pwm.getVitesseDefault(); // à changer pas bon -xavier
    uint8_t nbBoites = 0;
    bool loop = true;

    for(;loop;){
        
        lineTracker.updateValueMap();
        uint8_t valueMap = lineTracker.getValueMap();
        PORTC = valueMap;
        

        //TODO:  mettre dans une fonction
        
        //TODO: AJOUTER LE CAS VALUEMAP == 0
        
        //Conditions pour entrer dans la boite
        
        
        
        //TODO:  mettre dans une fonction
        switch(etat){
            case INIT:
                pwm.avancementAjuste(rapport, valueMap);
                if(valueMap == 3 || valueMap == 7 || valueMap == 15)
                    etat = TOURNE_GAUCHE;
            break;
            
            case TOURNE_GAUCHE:
                    //TODO:  mettre dans une fonction
                    
                    while(valueMap != 4){
                        lineTracker.updateValueMap();
                        valueMap = lineTracker.getValueMap();
                        pwm.tournantGauche(rapport, valueMap);
                    }
                    etat = LIGNE_DROITE;
                    
            break;

            case LIGNE_DROITE:
                //TODO:  mettre dans une fonction
                pwm.avancementAjuste(rapport, valueMap);
                if(valueMap == 15 || valueMap == 30 || valueMap == 31){
                    etat = PRE_BOITE;
                }
            break;

            case PRE_BOITE:
            //por le son
                PIEZO_INIT(DDD5, DDD7, 50);
                PLAY_NOTE(45);
                _delay_ms(50);
                setVolume(0);
                _delay_ms(20);
                setVolume(50);
                _delay_ms(50);
                setVolume(0);
                resetRegisters();

                while(valueMap == 15 || valueMap == 30 || valueMap == 31){
                    lineTracker.updateValueMap();
                    valueMap = lineTracker.getValueMap();
                    pwm.avancer(rapport);
                }
                etat = BOITE;
            break;

            case BOITE : 
                    //TODO: à mettre dans fonction à part
                    while( !(valueMap == 15 || valueMap == 30 || valueMap == 31) ){
                        lineTracker.updateValueMap();
                        valueMap = lineTracker.getValueMap();
                        pwm.boite(rapport, valueMap);
                    }
                    etat = POST_BOITE;
                    
                break;

            case POST_BOITE:
                //pour le son
                PIEZO_INIT(DDD5, DDD7, 50);
                PLAY_NOTE(45);
                _delay_ms(50);
                setVolume(0);
                _delay_ms(20);
                setVolume(50);
                _delay_ms(50);
                setVolume(0);
                resetRegisters();
                
                    nbBoites++;
                    while(valueMap == 15 || valueMap == 30 || valueMap == 31){
                        lineTracker.updateValueMap();
                        valueMap = lineTracker.getValueMap();
                        pwm.avancer(rapport);
                    }

                    if(nbBoites >= 3)
                        etat = NEXT;
                    else
                        etat = LIGNE_DROITE;
                break;
            
            case NEXT:
                    loop = false;
                    pointCounterISR = 0;
                break;
        }
        
    }
    
}

void Robot::RunCMDCoin(){
    enum etats {INIT, TOURNANT_GAUCHE, DROIT, AJUSTEMENT, TOURNER, NEXT};
    etats etat = INIT;
    uint8_t rapport = pwm.getVitesseDefault(); 
    uint16_t duree = 0xFFFF;
    uint8_t valueMap;
    uint16_t temps;
    bool loop = true;

    for(;loop;){
     
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
            pwm.arreter();
            _delay_ms(100);
            pwm.avancer(pwm.getVitesseDefault());
            if(valueMap == 0)
                etat = DROIT;
        break;

        case DROIT :
            startMinuterie(duree);
            pwm.avancer(pwm.getVitesseDefault());
            if(valueMap != 0){
                temps = TCNT1;
                etat = AJUSTEMENT;
                
                stopMinuterie();
                resetMinuterie();
            }

        break;

        case AJUSTEMENT:
            pwm.avancer(rapport);
            startMinuterie(duree);
            if(TCNT1 >= (temps) - (temps >> 2)) {
                etat = TOURNER;
                stopMinuterie();
                resetMinuterie();
            }
            

        break;

        case TOURNER:
            pwm.tourner90Precis(0,pwm.getVitesseDefault());
            etat = NEXT;
        break;

        case NEXT:
                loop = false;
        break;
                

        }
    }
}

