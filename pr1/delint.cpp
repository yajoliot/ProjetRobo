

/*
 * TP6-probleme 1
 * 
 * Auteurs: Stephanie mansour (1935595) et Yasmina Abou-Nakkoul (1897266)
 * Date: 16 octobre 2018
 * Fichier: pb1.cpp
 * 
 * Description: ce programme permet d'afficher de creer une suite 
 * de couleurs de la LED lorsqu'un interrupteur est pressé puis relâché,
 * ou quand l'interrupteur est pressé et que le compteur atteint 120. 
 * Tout ceci est fait à l'aide du principe de l'interruption (instruction ISR).
 * 			    
 */ 
 
  /*
 * Branchement pour le problème avec l'interrupteur:
 *  
 * On relie les fils rouge et noir avec le PORTB: le rouge est relié 
 * au Vcc et le noir est relié au Gnd.
 * Les fils vert et gris sont relié au PORTD: le vert est relié à 
 * la broche D3 et le gris est au D4.
 * La LED est reliée au PORTB : A1-A2.
 */

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> //POUR L'INTERRUPTEUR

const uint8_t SORTIE = 0xFF;
const uint8_t ENTREE = 0x00;
const uint8_t DEL_ETEINTE = 0x00;
const uint8_t DEL_ROUGE = 0x01;
const uint8_t DEL_VERTE = 0x02;
volatile uint8_t etat = 0;

ISR (INT0_vect) { 	// bon type de signal d'interruption en argument

	_delay_ms (30); // delai pour vérifier anti-rebond

	if(etat == 3) 	// changements d'états
		etat = 0;
	else etat++;
	
	/*si une seconde interruption intervient durant celle courante, l'AVR 
	s'en souvient et ISR sera executée après l'interruption courante*/
	EIFR |= (1 << INTF0); 
}

void initialisation (void) {
	cli (); //cli est une routine qui bloque toutes les interruptions 

	DDRD = ENTREE; 
	DDRA = SORTIE;
	
	//ajuste le registre EIMSK pour permettre les interruptions externes
	EIMSK |= (1 << INT0); 
	
	/*ajuste le registre EICRA pour sensibiliser les interruptions 
	externes aux changements de niveau */
	EICRA |= (1 << ISC00); 

	sei (); // sei permet de recevoir à nouveau des interruptions.
}


int main() {
	int compteur = 0;
	initialisation();

	while (true) {
		switch (etat) {	
			case 0: 
				PORTA = DEL_ETEINTE;		
				break;

			case 1: 
			/*incrementer le compteur si, lorsque le bouton est pesé,
			  sa valeur est plus petite que 120*/
				if(compteur<120){ 
					compteur +=10;
					_delay_ms(1000);
				} else etat = 2;
				break;

			case 2: 
			// faire clignoter la DEL en vert pendant une demi seconde
				for (int i=0; i<2;i++) { 
					PORTA = DEL_VERTE;
					_delay_ms(150);
					PORTA=DEL_ETEINTE;
					_delay_ms(100);		
				}	
				etat = 3;				
				break;

			case 3: 
				//delai de 2 sec
				_delay_ms(2000);											
				
				/* faire clignoter la DEL en rouge (compteur / 2) fois 
				   au rythme de 2 fois par seconde*/
				for (int i = 0; i < (compteur/10); i++){
					for (int j = 0; j < 5; j++) {
						PORTA = DEL_ROUGE;
						_delay_ms(125);
						PORTA = DEL_ETEINTE;
						_delay_ms(75);
					}
				}
				PORTA = DEL_VERTE;
				_delay_ms(1000); 		//délai de 1 sec 
				PORTA = DEL_ETEINTE;
				etat = 0;
				break;
		}
	}
	return 0;
}