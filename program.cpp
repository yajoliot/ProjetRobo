#define 	F_CPU   1000000UL


#include "../TP1/antiRebond.cpp"
#include <avr/io.h> 
#include <util/delay.h>

/*void DelayMs(int n){
        while (n--){
            _delay_ms(1);
        }
}*/

int main()
{
  
    DDRB = 0xff; // PORT B est en mode sortie
    
    int delay = 4000;
    PORTB = 2;

    if(antiRebond()){
        PORTB = 1;
        for(;;){

            PORTB = 1;

            /*DelayMs(delay);
            delay--;
            PORTB = 0;
            */
            
        }
    }
    

    

    return 0;
}