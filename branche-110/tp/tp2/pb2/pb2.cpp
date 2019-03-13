/**
 * Nom 				: TP2 - Problème 2 
 * fichier 			: tp2_probleme2.cpp
 * Auteurs 			: Simon Zhang (1839892) & Maria Jabrane (1846697)
 * Numéro d'équipe	: #110
 * version			: Derniere entree 25-01-2019
 * 
 * Description: 
 * Quand la carte mère démarre, la DEL libre doit s'allumer en rouge. 
 * Si le bouton-poussoir noir est pesé, la DEL affiche la couleur ambre. 
 * Quand le bouton-poussoir est relâché, la DEL devient verte. 
 * Si le bouton est de nouveau pesé, la DEL prend la couleur rouge encore. 
 * Quand il est relâché, la DEL s'éteint. 
 * Si le bouton est de nouveau pesé, la DEL affiche la couleur verte. 
 * Quand il est relâché, la DEL tourne au rouge ce qui fait que la carte mère 
 * est de retour à son état initial et tout peut recommencer.
 * //source  : Document de reference pour le TP2
 * 
 * Configuration : 
 * PIN 1 du PORT B brancheé à la borne positive de la DEL libre,
 * PIN 2 du PORT B brancheé à la borne négative.
 * Le bouton est relié par un cavalier à la PIN 3 du PORT D.  
 * 
 * Nous avons de nouveau utilisé la machine de Mealy. Nous avons la même entrée,
 * toutefois nous avons 3 sorties pour chacune des couleurs de la DEL.
 * 
 * Table des états : 
 * 
 * | État présent | BoutonPese 	  | État suivant | Rouge | Ambrée | Verte |
 * |--------------|---------------|--------------|-------|--------|-------|
 * | INIT         | 0             | INIT         | 1     | 0      | 0     |
 * | INIT         | 1             | E1           | 0     | 1      | 0     |
 * | E1           | 0             | E1           | 0     | 0      | 1     |
 * | E1           | 1             | E2           | 1     | 0      | 0     |
 * | E2           | 0             | E2           | 0     | 0      | 0     |
 * | E2           | 1             | INIT         | 0     | 0      | 1     |
 * 
 */
 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

enum state {INIT, E1, E2};

const uint8_t GREEN = 0x02;
const uint8_t RED = 0x01;
const uint8_t LIGHT_OFF = 0x00;


/*************************************************************************************
 * Nom 				: boutonPese
 * Paramètres 		: aucun
 * Valeur de retour : booléen
 * Description 		: Retourne true si le bouton est pesé (1 fois seulement), 
 * 					  Retourne false si le bouton n'est pas pesé.
 * 
 ************************************************************************************/ 
bool boutonPese() {
	if (PIND & 0x04) {
		_delay_ms(10); // anti-rebond 
		if (PIND & 0x04)
			return true;
	}
	return false;
}


/************************************************************************************
 * Nom 				: amberColor
 * Paramètres 		: aucun
 * Valeur de retour : void
 * Description		: Tant que le bouton reste pesé, allumer la DEL en ambré 
 * 
 ***********************************************************************************/ 
void amberColor() {
	do {
		PORTB = RED;
		_delay_ms(2);
		PORTB = GREEN;
		_delay_ms(10);
	} while (boutonPese());
}


int main()
{
	DDRB = 0xff; // PORT B est en mode sortie. (DEL)
	DDRD = 0x00; // PORT D est en mode entrée. (bouton)
	
	state etatPresent = INIT;
		
	for(;;)
	{		
		switch (etatPresent) {
			case INIT : if (!boutonPese()) {
							PORTB = RED;
						}
						else {
							amberColor();
							etatPresent = E1;
						}
						break;
			
			case E1 :	if (!boutonPese()) {
						PORTB = GREEN;
						}
						else {
							while (boutonPese()) { 
								PORTB = RED;
							}
						etatPresent = E2;
						}
						break;
			
			case E2 : 	if (!boutonPese()) {
							PORTB = LIGHT_OFF;
						}
						else {
							while (boutonPese()) {
								PORTB = GREEN;
								}
							etatPresent = INIT;
							}
					  	break;
		}
	}
	
	return 0;
}