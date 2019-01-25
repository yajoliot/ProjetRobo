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
    
    int delay = 400;
    int delay2 = 400;

    /*for (;;){
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
    }*/


    for(;;){

        while(delay != 0){
            PORTB = 1;
            DelayMs(delay);
            PORTB = 0;
            DelayMs(delay2);
            delay--;
        }

        while (delay != 400){
            PORTB = 0;
            DelayMs(delay2);
            PORTB = 2;
            DelayMs(delay);
            delay++;

        }

        while(delay != 0){
            PORTB = 1;
            DelayMs(delay);
            PORTB = 0;
            DelayMs(delay2);
            delay--;
        }

        while (delay != 400){
            PORTB = 0;
            DelayMs(delay2);
            PORTB = 2;
            DelayMs(delay);
            delay++;

        }

    }

     /*delay = 50;
     delay2 = 50;

    for(;;){

        while(delay != 0){
            PORTB = 2;
            DelayMs(delay);
            PORTB = 0;
            delay--;
        }

        while (delay2 != 0){
            PORTB = 0;
            DelayMs(delay);
            PORTB = 2;
            delay2--;

        }

         
     
    }*/
    

   
        
        

   return 0;


}