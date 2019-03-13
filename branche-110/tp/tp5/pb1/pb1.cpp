#define F_CPU 8000000
#define INTERRUPTEUR_ON (PIND & 0x04)
#include <stdint.h>
#include <avr/io.h> 
#include <util/delay.h>

const uint8_t VERT = 0x02;
const uint8_t ROUGE = 0x01;
const uint8_t ETEINTE = 0x00;

//Lorsque a/b est egal a 1, la del est allume
// On  veut modifier b pour que a/b tende progressivement vers 0
// Notre strategie est de modifier a 
void attenuerDEL(const uint8_t COULEUR){
    double a = 1; // Valeur initiale pour avoir un ratio a/b de 1
    const double b = 10000; // Ceci correspond a la periode
   // while( (b - a) < 76 ){
   for (int i = 0; i < 300; i++){
        PORTC=COULEUR;
        _delay_loop_2(b-a);     
         PORTC = ETEINTE;
        _delay_loop_2(a+b);
        a=a+(33.33);

        }
    
}   
int main(){
    DDRC = 0xff; //Port en sortie
    DDRB = 0x00; //Port en entree
    for(;;){
        attenuerDEL(ROUGE);
        attenuerDEL(VERT);
    }
}
