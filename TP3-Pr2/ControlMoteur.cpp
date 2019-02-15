#define 	F_CPU   8000000


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
    
    //int delayA = 0;
    //int delayB = 0;
    int temps = 0;


    

    
    _delay_ms(10000);
        
        

    
    while(temps != 120){
        PORTB = 2;
        _delay_ms(4.7*5);
        PORTB = 0;
        _delay_ms(12.5*5);
        temps++;
    }

     
    while(temps != 240){
        PORTB = 2;
        _delay_ms(8*5);
        PORTB = 0;
        _delay_ms(8*5);
        temps++;
    }
    
    while(temps != 360){
        PORTB = 2;
        _delay_ms(12.5*5);
        PORTB = 0;
        _delay_ms(4.7*5);
        temps++;
    }

    while(temps != 480){
        PORTB = 2;
        _delay_ms(16.666*5);
        temps++;
    }

    PORTB = 0;
    _delay_ms(10000);
    temps = 0;
while(temps != 400){
        PORTB = 2;
        _delay_ms(1.25*5);
        PORTB = 0;
        _delay_ms(3.75*5);
        temps++;
    }

     
    while(temps != 800){
        PORTB = 2;
        _delay_ms(2.5*5);
        PORTB = 0;
        _delay_ms(2.5*5);
        temps++;
    }
    
    while(temps != 1200){
        PORTB = 2;
        _delay_ms(3.75*5);
        PORTB = 0;
        _delay_ms(1.25*5);
        temps++;
    }

    while(temps != 1600){
        PORTB = 2;
        _delay_ms(5*5);
        temps++;
    }
    
    PORTB=0;
 

    

   return 0;


}
 /*while (delay != 400){
            PORTB = 0;
            DelayMs(delay2);
            PORTB = 2;
            DelayMs(delay);
            delay++;
        }

        int delay2 = 500;
        _delay_ms(2000);

        while (delay != 300){
            PORTB = 0;
            DelayMs(delay2);
            PORTB = 2;
            DelayMs(delay);
            delay++;
        }

        int delay2 = 500;
        _delay_ms(2000);

        while (delay != 200){
            PORTB = 0;
            DelayMs(delay2);
            PORTB = 2;
            DelayMs(delay);
            delay++;
        }

        int delay2 = 500;
        _delay_ms(2000);

        while (delay != 100){
            PORTB = 0;
            DelayMs(delay2);
            PORTB = 2;
            DelayMs(delay);
            delay++;
        }

        _delay_ms(2000);*/