//EN-TETE A COMPLETER
#define F_CPU 8000000
#define INTERRUPTEUR_ON (PIND & 0x04)
#include <stdint.h>
#include <avr/io.h> 
#include <util/delay.h>
//CONSTANTES GLOBALES
  const uint8_t VERT = 0x01; //on ne prend en compte que 1-2, pas les autres series de bits,
                                 //afin de simplifier notre tache.
  const uint8_t ROUGE = 0x02;
  const double DELAI = 3000; //trois secondes de delai nous semble correct pour
					         //que l'utilisateur voit le changement
  const uint8_t DELAIMINI = 1;
//FONCTION POUR LA COULEUR JAUNE
void rendreAmbre(int tempsPris){
	for (int chronometre = 0; chronometre < tempsPris; chronometre++){ //La lumiere est jaune pendant trois secondes
			PORTB = VERT;
			_delay_ms(DELAIMINI+2);
			PORTB = ROUGE;
			_delay_ms(DELAIMINI);
	}
 }
 
int main()
{
  DDRA = 0xff; // PORT A est en mode sortie
  DDRB = 0xff; // PORT B est en mode sortie
  DDRC = 0xff; // PORT C est en mode sortie
  DDRD = 0x00; // PORT D est en mode entree
  //unsigned long compteur=0; // le compteur est initialise a 0.
                            // c'est un compteur de 32 bits
  for(;;){
	 
		 if(INTERRUPTEUR_ON){
       _delay_ms(10);
       if(INTERRUPTEUR_ON)
         rendreAmbre(1500); //La lumiere sera ambre tant qu'on enfonce le bouton avec une latence de 2ms, vu notre definition precedente de la fonction.
    }
     else{
       PORTB = ROUGE;
       _delay_ms(DELAI);
	     rendreAmbre(1500);
	     PORTB = VERT;
	     _delay_ms(DELAI);
    }

	}
  return 0;
}
