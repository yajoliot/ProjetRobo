#include "couleurDel.h"

void allumerDEL(uint8_t Couleur){

    if(Couleur == ETEIND){
        PORTB = ETEIND;
    }else if (Couleur == ROUGE){
        PORTB = ROUGE;
    }else if (Couleur == VERT){
        PORTB = VERT;
    }else if (Couleur == AMBRE)
    {
        PORTB=0x01;
		_delay_ms(6);
		PORTB=0x02;
        _delay_ms(9);
    }
    
}

