/*
  * Nom 			: TP2 - Problème 1 
 * fichier 			: tp2_probleme1.cpp
 * Auteurs 			: Simon Zhang (1839892) & Maria Jabrane (1846697)
 * Numéro d'équipe	: #110
 * version			: Derniere entree 25-01-2019
 * 
 * Description: 
 * La DEL est éteinte au départ. On doit appuyer et relâcher 5 fois 
 * le bouton-poussoir avant que la DEL tourne au rouge pendant exactement 1 seconde. 
 * Par la suite, on revient au départ pour pouvoir recommencer.
 *  
 * Entrées : 
 * Nous avons branché la PIN 1 du PORT B à la borne positive de la DEL libre,
 * et la PIN 2 du PORT B à la borne négative.
 * Le bouton est relié avec un cavalier à la PIN 3 du PORT D.
 * 
 * Nous avons utilisé le concept d'une machine de Mealy. 
 * Entrée boutonPese (true si oui/ false si non) 
 * Sortie Z :représente la DEL qui s'allume en rouge 
 * pendant 1 seconde OU ne s'allume pas .
 * 
 * Table des états :
 * 
 * | État présent | boutonPese    | État suivant | Sortie Z |
 * |--------------|---------------|--------------|----------|
 * | INIT         |             0 | INIT         |        0 | DEL éteinte
 * | INIT         |             1 | E1           |        0 |
 * | E1           |             0 | E1           |        0 |
 * | E1           |             1 | E2           |        0 |
 * | E2           |             0 | E2           |        0 |
 * | E2           |             1 | E3           |        0 |
 * | E3           |             0 | E3           |        0 |
 * | E3           |             1 | E4           |        0 |
 * | E4           |             0 | E4           |        0 |
 * | E4           |             1 | E5           |        0 |
 * | E5 		  |             0 | E5           |        0 |
 * | E5           |             1 | INIT         |        1 |DEL allumée
 *  
 */


#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

enum state {INIT, E1, E2, E3, E4, E5};


/****************************************************************************
 * Nom 				: estPresse
 * Paramètres 		: aucun
 * Valeur de retour : booléen
 * Description 		: selon les entrées : 
 *					  Retourne true si le bouton a été pressé PUIS relâché. 
 * 					  Retourne false si le bouton n'est pas pressé.
 * 
 ****************************************************************************/
bool estPresse() {
	if (PIND & 0x04) {
		_delay_ms(10); 			// anti-rebond
		while (PIND & 0x04) {   // TANT QUE le bouton est pressé, FAIRE Busy-Wait
		}  
		return true;
	}
	return false;
}


int main()
{
	DDRB = 0xff; // PORT B est en mode sortie (DEL)
	DDRD = 0x00; // PORT D est en mode entrée (bouton)
	
	const uint8_t RED = 0x01;          // DEL allumée en rouge
	const uint8_t LIGHT_OFF = 0x00;	   // DEL éteinte
	
	state etatPresent = INIT;
	
	for(;;)
	{
		switch (etatPresent) {
			case INIT : if (estPresse()) 
							etatPresent = E1; 
						break;
			
			case E1 : if (estPresse()) 
						  etatPresent = E2; 
					  break;
			
			case E2 : if (estPresse()) 
						  etatPresent = E3; 
					  break;
			
			case E3 : if (estPresse()) 
						 etatPresent = E4;
					  break;
		
			case E4 : if (estPresse()) 
						  etatPresent = E5; 
					  break;
			
			case E5 : if (estPresse()) {
						PORTB = RED;       // DEL allumée en rouge
						_delay_ms(1000);  //  pendant 1 seconde
						PORTB = LIGHT_OFF;
						etatPresent = INIT;
					  }; 
					  break;
		}
	}

	return 0;
}