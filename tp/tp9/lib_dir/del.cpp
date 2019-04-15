#include "del.h"

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

void allumerDEL_dal(uint8_t operande){
  if (operande == 0x01){ // Allume en vert
    allumerDEL(VERT);
  }
  else if(operande == 0x02){ // Allume en rouge
    allumerDEL(ROUGE);
  }
}
