
#define 	F_CPU   8000000UL



#include <avr/io.h> 
#include <util/delay.h>


const int ETEIND = 0; //DEL ETEINTE
const int ROUGE = 1; //DEL ROUGE
const int VERT = 2; //DEL VERTE

DDRD = 0x00;
DDRB = 0xff;
int compteur = 0;

int main(){

    PORTB = PORTD;
    
    /*
    while(antiRebond || compteur != 120){
        _delay_ms(100);
        compteur++;
    }

    PORTB = VERT;
    _delay_ms(500);
    PORTB = ETEIND;
    _delay_ms(2000);

    for(int i = 0; i < compteur/2; i++){
        PORTB = ROUGE;
        _delay_ms(250);
        PORTB = ETEIND;
        _delay_ms(250);
        PORTB = ROUGE;
        _dela_ms(250);
        PORTB = ETEIND;
        _delay_ms(250);
    }

     PORTB = VERT;
     _delay_ms(1000);
     PORTB = ETEIND;
     */




   



    

    

    
    


   return 0;

}