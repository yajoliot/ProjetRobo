#define 	F_CPU   1000000UL


#include "../TP1/antiRebond.cpp"
#include <avr/io.h> 
#include <util/delay.h>

const int ETEIND = 0; //DEL ETEINTE
const int ROUGE = 1; //DEL ROUGE
const int VERT = 2; //DEL VERTE

void DelayMs(int n){
    while (n--){
        _delay_ms(1);
    }
}

int main(){


        
    DDRB = 0xff; // PORT B est en mode sortie
    
    int delay = 50;
    for (;;){
    if (antiRebond()){
        for(;;){

            PORTB = 1;
            DelayMs(delay);
            PORTB = 0;
            DelayMs(delay);
            delay -= 1;

            if(delay == 0){
                PORTB = 0;

            }

        }
    }
    }
    

   
        
        

   return 0;


}