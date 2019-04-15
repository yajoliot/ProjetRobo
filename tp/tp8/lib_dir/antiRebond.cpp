#define 	F_CPU   1000000UL

#include <avr/io.h> 
#include <util/delay.h>



bool antiRebond(){

    
    bool interrupteur = false; // est a 1 si l'interrupteur est activ/
	
    if ( !(PIND & 0x04) ) {

        _delay_ms(10);

        if ( !(PIND & 0x04) ){
                interrupteur = false;
        }
    }

    if (PIND & 0x04) {

        _delay_ms(10);

        if (PIND & 0x04){
            interrupteur = true;
        }
    }

    
    return interrupteur;
}