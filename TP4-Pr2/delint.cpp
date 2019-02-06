
#define 	F_CPU   8000000UL


#include "../TP1/antiRebond.cpp"
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

const int ETEIND = 0; //DEL ETEINTE
const int ROUGE = 1; //DEL ROUGE
const int VERT = 2; //DEL VERTE

enum etats {INIT,E1,E2,E3,E4,S};
volatile int etat = INIT;


void isr_INIT() {

    DDRD = 0x00;
    DDRB = 0xff;

    
    cli ();

    EIMSK |= (1 << INT0);

    EICRA |= (1 << ISC01); // EICRA = EICRA | (1 << ISC01)


    sei ();


}

ISR(INT0_vect){


   _delay_ms(30);

    EIFR |= (1 << INTF0);
    PORTB = VERT;
}

int main(){

    isr_INIT();


    _delay_ms(50000);

    PORTB = ROUGE;
    _delay_ms(1000);
    PORTB = ETEIND;
    
    _delay_ms(10000);

    cli();

    if(PORTB != VERT)
        PORTB = ROUGE;
    

    
    


   return 0;

}