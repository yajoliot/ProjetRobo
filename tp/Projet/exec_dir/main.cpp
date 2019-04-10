#ifndef F_CPU
 #define F_CPU 8000000UL
 #endif

 #include <util/atomic.h>
 #include <util/delay.h>
 #include <stdlib.h>
 #include <avr/interrupt.h>
 
 void isr_INIT() {

    DDRD = 0x00;
    DDRB = 0xff;

    
    cli ();

    EIMSK |= (1 << INT0);

    EICRA |= (1 << ISC01) | (1 << ISC00); // EICRA = EICRA | (1 << ISC01)

    sei();
}

//ISR POUR SECTION3 (RunCMD3())
ISR(INT0_vect){
    _delay_ms(30);
    if(PIND & 0x04){
        //etat = ANALYSE;
    }
    
}

int main() {

    for(;;){}
    
}