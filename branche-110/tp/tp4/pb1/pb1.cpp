#define F_CPU 8000000
#define INTERRUPTEUR_ON (PIND & 0x04)
#define INTERRUPTEUR_OFF !(PIND & 0x04)
#include <stdint.h>
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
//CONSTANTES GLOBALES

  const uint8_t ETEINTE = 0x00; //pas de couleur
  const uint8_t ROUGE = 0x01; //on ne prend en compte que 1-2, pas les autres series de bits,
                             //afin de simplifier notre tache.
  const uint8_t VERT = 0x02;
  const double DELAI = 1000; //une seconde
  volatile uint8_t compteur = 0;
  const uint8_t DELAIMINI = 1;
//FONCTION POUR LA COULEUR JAUNE
void rendreAmbre(int tempsPris){
	for (int chronometre = 0; chronometre < tempsPris; chronometre++){ //La lumiere est jaune pendant trois secondes
			PORTC = VERT;
			_delay_ms(DELAIMINI+2);//+2 sert a rapprocher la couleur du jaune plutot qu'elle soit orangee
			PORTC = ROUGE;
			_delay_ms(DELAIMINI);
	}
 }
 void rendreAmbreIllimite(){
   while (INTERRUPTEUR_ON){
      PORTC = VERT;
			_delay_ms(DELAIMINI+2);//+2 sert a rapprocher la couleur du jaune plutot qu'elle soit orangee
			PORTC = ROUGE;
			_delay_ms(DELAIMINI);
   }
 }
bool boutonOnDebounce(){ //Fait l'antirebond si l'interrupteur est active.
     if (INTERRUPTEUR_ON){
         _delay_ms(10);
         if (INTERRUPTEUR_ON)
             return true;
     } return false;
}
bool boutonOffDebounce(){ //antirebond de l'interrupteur off.
 if(INTERRUPTEUR_OFF){
             _delay_ms(10);
             if(INTERRUPTEUR_OFF){
                 return true;
             }
        } return false;
}

// placer le bon type de signal d'interruption

// à prendre en charge en argument

ISR (INT0_vect) {

// laisser un delai avant de confirmer la réponse du

// bouton-poussoir: environ 30 ms (anti-rebond)

_delay_loop_2 ( 30 );

// se souvenir ici si le bouton est pressé ou relâché

 if((boutonOnDebounce() && (compteur == 0 || compteur == 2 || compteur == 4)) || (boutonOffDebounce() && (compteur == 1 || compteur == 3 || compteur == 5))){
         compteur++;  

// changements d'états tels que ceux de la

// semaine précédente

// Voir la note plus bas pour comprendre cette instruction et son rôle

EIFR |= (1 << INTF0) ;

}
}
void initialisation ( void ) {

// cli est une routine qui bloque toutes les interruptions.

// Il serait bien mauvais d'être interrompu alors que

// le microcontroleur n'est pas prêt...

cli ();


// configurer et choisir les ports pour les entrées

// et les sorties. DDRx... Initialisez bien vos variables

DDRC = 0xff; // PORT C est en mode sortie
DDRD = 0x00; // PORT D est en mode entree


// cette procédure ajuste le registre EIMSK

// de l’ATmega324PA pour permettre les interruptions externes

EIMSK |= (1 << INT0) ;


// il faut sensibiliser les interruptions externes aux

// changements de niveau du bouton-poussoir

// en ajustant le registre EICRA

EICRA |= 0b010101 ; //PAGE 67. Triggers interrupt on rising edge AND on falling edge.


// sei permet de recevoir à nouveau des interruptions.

sei ();

}




int main(){
  initialisation();
 // volatile uint8_t compteur = 0;         MIS EN GLOBAL. Cela devrait-il être différent??
  PORTC = ETEINTE; 
  for (;;){
  switch (compteur)
	  {
	  case 1: rendreAmbreIllimite();
		  break;
	  case 2: PORTC = VERT;
		  break;
	  case 3: PORTC = ROUGE;
		  break;
	  case 4: PORTC = ETEINTE;
		  break;
	  case 5: PORTC = VERT;
		  break;
    case 6: compteur = 0;
	  default: PORTC = ROUGE;
		  break;
	  }
  }
}
    
    

