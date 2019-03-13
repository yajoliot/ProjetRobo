#define 	F_CPU   1000000UL


#include "../TP1/antiRebond.cpp"
#include <avr/io.h> 
#include <util/delay.h>

const int ETEIND = 0; //DEL ETEINTE
const int ROUGE = 1; //DEL ROUGE
const int VERT = 2; //DEL VERTE

int main(){

    enum etats {INIT,E1,E2};
    int etat = INIT;
    DDRB = 0xff;
        
        
        
    for (;;){
        switch(etat){
        case INIT:
            PORTB = ROUGE;
            if(antiRebond()){
                 for(;;){
                    PORTB = ROUGE;
                    _delay_ms(25);
                    PORTB = VERT;
                    _delay_ms(150); //couleur ambre est alterner rapidement entre 
                                    //la couleur rouge et verte

                     if(!antiRebond()){
                        etat = E1;
                        break; 
                     }     
                 }
            }  
        break;

        case E1:  
            PORTB = VERT; 
            if(antiRebond()){
                 for(;;){
                     PORTB = ROUGE;
                     if(!antiRebond()){
                         PORTB = ETEIND;
                        etat = E2;
                        break; 
                     }      
                 }
            }
        break;

        case E2:  
            
            if(antiRebond()){
                PORTB = VERT;
                 for(;;){
                     if(!antiRebond()){
                        etat = INIT;
                        break; 
                     }      
                 }
            }
        break;

    }

   
}    
        

   return 0;


}