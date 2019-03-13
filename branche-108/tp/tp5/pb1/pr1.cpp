
#define 	F_CPU   8000000UL


#include "../TP1/antiRebond.cpp"
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>


#include "memoire_24.h"

const int ETEIND = 0; //DEL ETEINTE
const int ROUGE = 1; //DEL ROUGE
const int VERT = 2; //DEL VERTE

enum etats {INIT,E1,E2,E3,E4,S};
volatile int etat = INIT;




int main(){

    DDRB = 0xff;
    uint16_t adresse = 0x00;
    uint8_t tableauPoly[] = {'*','P','*','O','*','L','*','Y','*','T','*','E','*','C','*','H','*','N','*','I','*','Q','*','U','*','E','*',' ','*','M','*','O','*','N','*','T','*','R','*','E','*','A','*','L','*'};
    uint8_t taille = sizeof(tableauPoly);
    uint8_t lecture[46] = {};


    Memoire24CXXX memoire = Memoire24CXXX();
    //memoire.init();
    memoire.ecriture(adresse, tableauPoly, taille);
    _delay_ms(10);
    memoire.lecture(adresse, lecture, taille);
    
    bool bon = false;

    for(int i = 0; i < sizeof(tableauPoly); i++){
        if(tableauPoly[i] == lecture[i]){
            PORTB = VERT;
        }else{
            PORTB = ROUGE;
        }

        if(PORTB == ROUGE){
            break;
        }

    }


   return 0;

}