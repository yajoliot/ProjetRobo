
#define 	F_CPU   8000000UL


#include "../TP1/antiRebond.cpp"
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

const int ETEIND = 0; //DEL ETEINTE
const int ROUGE = 1; //DEL ROUGE
const int VERT = 2; //DEL VERTE

volatile uint8_t minuterieExpiree;

volatile uint8_t boutonPoussoir; 



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
    boutonPoussoir = 1;
    

}

ISR(TIMER1_COMPA_vect){
   minuterieExpiree = 1;
}




void partirMinuterie ( uint16_t duree ) {

minuterieExpiree = 0;

// mode CTC du timer 1 avec horloge divisée par 1024

// interruption après la durée spécifiée

TCNT1 = 0 ;

OCR1A = duree;

TCCR1A |= (1 << COM1A1);
TCCR1A |= (1 << COM1A0);
TCCR1B |= (1 << WGM10) ;
TCCR1B |= (1 << CS10) ;
TCCR1B |= (1 << CS12) ;
TCCR1C = 0;

TIMSK1 |= (1 << OCIE1A)  ;

}

int main(){
    isr_INIT();
    
    
    partirMinuterie(25);

    if(minuterieExpiree == 1)
        PORTB = 1;
    if(boutonPoussoir == 1)
        PORTB = 2;
    
    

    
    


   return 0;

}