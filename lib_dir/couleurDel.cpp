#include <couleurDel.h>

const uint8_t ETEIND = 0x00;
const uint8_t ROUGE = 0x01;
const uint8_t VERT = 0x02;
const uint8_t AMBRE = 0x03;

allumerDel(couleur){

    if(couleur == ETEIND){
        PORTB = ETEIND;
    }else if (couleur == ROUGE){
        PORTB = ROUGE;
    }else if (couleur == VERT){
        PORTB = VERT;
    }else if (couleur == AMBRE)
    {
        PORTB=0x01;
		_delay_ms(4);
		PORTB=0x02;
        _delay_ms(10);
    }
    
}

