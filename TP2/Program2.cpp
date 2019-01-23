#define 	F_CPU   1000000UL


#include "../TP1/antiRebond.cpp"
#include <avr/io.h> 
#include <util/delay.h>

int main(){

    enum etats {INIT,E1,E2,E3,E4,S};
    int etat = INIT;
    DDRB = 0xff;

        
        

        switch(etat){
        case INIT:
             while(antiRebond()){
                if (!antiRebond()) etat = E1;   
            }
        
            
        break;

        case E1:   
        
        while(antiRebond()){
                if (!antiRebond()) etat = E2;
        }
            
        break;
        case E2:   

        while(antiRebond()){
                if (!antiRebond()) etat = E3;   
            }
            
        break;

        case E3:   

         while(antiRebond()){
                if (!antiRebond()) etat = E4;   
            }
            
        break;

        case E4:   

       while(antiRebond()){
                if (!antiRebond()) etat = S;   
            }
            
        break;

        case S:    
        
        while(antiRebond()){
            if (!antiRebond()){
                PORTB = 1; 
                _delay_ms(1000); 
                PORTB = 0;
                etat = INIT;
            } 
        }

        break;
    }

    
    
        

   return 0;


}