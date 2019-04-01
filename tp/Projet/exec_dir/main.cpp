#ifndef F_CPU
 #define F_CPU 8000000UL
 #endif
 
 #include <util/atomic.h>
 #include <util/delay.h>
 #include "pwm.h"
 #include "piezo.h"
 #include "usart.h"
 #include "memoire_24.h"
 #include "usart.h"
 #include "bytecode.h"
 #include "util.h"
 #include "linetracker.h"
 #include "debug.h"

int main() {
    PWM pwm;
    LineTracker lineTracker;
    enum etats {LIGNE_DROITE, TOURNE_GAUCHE, ARRETE};
    int etat = LIGNE_DROITE;
    DDRC = 0xFF;
    DDRB = 0xFF;
    uint8_t rapport = 140; // à changer pas bon -xavier

    for(;;){
        
        lineTracker.updateValueMap();
        uint8_t valueMap = lineTracker.getValueMap();
        PORTC = valueMap;

        //TODO:  mettre dans une fonction
        if( valueMap == 4 || valueMap == 6 ||valueMap == 12 || valueMap == 8 || valueMap == 2 || valueMap == 3 || valueMap == 24 ||valueMap == 1 || valueMap == 16)
            etat = LIGNE_DROITE;
        else if(valueMap == 7 || valueMap == 15 || valueMap == 1 || valueMap == 0)
            etat = TOURNE_GAUCHE;
        else 
            etat = ARRETE;
        
        //TODO:  mettre dans une fonction
        switch(etat){
            case LIGNE_DROITE:
                    pwm.avancementAjuste(rapport, valueMap);
                break;
            
            case TOURNE_GAUCHE:
                    pwm.tournantGauche(rapport, valueMap);
                break;
            
            case ARRETE:
                    pwm.arreter();
                break;
        }
        
    }
}


/*These define lines can be in the util.h or the constantes.h file (probably constantes.h file)
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>



#define OPEN 0xFF
#define MAX_5_BIT 0x1F
#define DELAY_TIME 100


int main(){
    // DDRC = OPEN;
    DDRC = MAX_5_BIT;
    
    // This for loop will test all the combinations between pins 1 to 5
    for(uint8_t count=0 ; ;count++){
        PORTC = count;
        _delay_ms(DELAY_TIME);
        // variableDelay(DELAY_TIME); //could just use the _delay_ms() function here since this is not a variable
        if(count==MAX_5_BIT)
            count = 0x00	;
    }
    // It will follow the following sequence
    // 1. Nothing
    // 2. DEL1
    // 3. DEL2
    // 4. DEL1 && DEL2
    // 5. DEL3
    // 6. DEL3 && DEL1
    // and so on...
}
*/
