#define 	F_CPU   1000000UL


#include "../TP1/antiRebond.cpp"
#include <avr/io.h> 
#include <util/delay.h>


const int ETEIND = 0; //DEL ETEINTE
const int ROUGE = 1; //DEL ROUGE
const int VERT = 2; //DEL VERTE

int main(){

    enum etats {INIT,E1,E2,E3,S};
    int etat = INIT;
    DDRB = 0xff;
        
        
        
    for (;;){
        switch(etat){
        case INIT:
            if(antiRebond()){
                 for(;;){
                     if(!antiRebond()){
                        etat = E1;
                        break; 
                     }     
                 }
            }  
        break;

        case E1:   
            if(antiRebond()){
                 for(;;){
                     if(!antiRebond()){
                        etat = E2;
                        break; 
                     }      
                 }
            }
        break;

        case E2:
           

            if(antiRebond()){
                 for(;;){
                     if(!antiRebond()){
                        etat = E3;
                        break; 
                     }     
                 }
            }
        break;

        case E3: 
            if(antiRebond()){
                 for(;;){
                     if(!antiRebond()){
                        etat = S;
                        break; 
                     }       
                 }
            }
        break;


        case S:    
            if(antiRebond()){
                 for(;;){
                     if(!antiRebond()){
                        etat = INIT;
                        PORTB = ROUGE;
                        _delay_ms(1000);
                        PORTB = ETEIND;
                        break; 
                     }
                          
                 }
                 

            }
        }
    }

    
    
        

   return 0;


}