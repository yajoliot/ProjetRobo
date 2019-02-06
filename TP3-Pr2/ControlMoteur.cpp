#define 	F_CPU   1000000UL


#include "../TP1/antiRebond.cpp"
#include <avr/io.h> 
#include <util/delay.h>

const int ETEIND = 0; //DEL ETEINTE
const int ROUGE = 1; //DEL ROUGE
const int VERT = 2; //DEL VERTE

void DelayMs(int n){
    while (n!=0){
        _delay_ms(0.1);
        n--;
    }
}

int main(){


        
    DDRB = 0xff; // PORT B est en mode sortie
    
    int delay = 500;
    int delay2 = 500;


    for(;;){

        PORTB = 1;

        while (delay != 400){
            PORTB = 0;
            DelayMs(delay2);
            PORTB = 1;
            DelayMs(delay);
            delay++;
        }

        int delay2 = 500;
        _delay_ms(2000);

        while (delay != 300){
            PORTB = 0;
            DelayMs(delay2);
            PORTB = 1;
            DelayMs(delay);
            delay++;
        }

        int delay2 = 500;
        _delay_ms(2000);

        while (delay != 200){
            PORTB = 0;
            DelayMs(delay2);
            PORTB = 1;
            DelayMs(delay);
            delay++;
        }

        int delay2 = 500;
        _delay_ms(2000);

        while (delay != 100){
            PORTB = 0;
            DelayMs(delay2);
            PORTB = 1;
            DelayMs(delay);
            delay++;
        }

        _delay_ms(2000);
 

    }

   return 0;


}