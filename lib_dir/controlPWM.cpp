#include <controlPWM.h>

void initPWM(){ 
	// son constructeur par defaut initailise les registres TCCR1A,
	// TCCR1B et TCCR1C correctement

	// mise à un des sorties OC1A et OC1B sur comparaison
	// réussie en mode PWM 8 bits, phase correcte
	// et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6
	// page 177 de la description technique du ATmega324PA)

	TCNT1 = 0;

	TCCR1A |= (1 << COM1A1) | (1<< COM1B1);
	TCCR1A |= (1 << WGM10);
	TCCR1B |= (1 << CS11);

	TCCR1C = 0;

}


// Ajustement de la vitesse des roues


/**********************************************************************************
* Methode : roueGauche (bool direction, uint8_t rapport )
* Role : Ajuster la vitesse et la direction de la roue gauche
* Parametre : - rapport, correspond au pourcentge de la vitesse max du robot(100%, 50%...)
			  - direction, correspond a l direction du robot (true avance,
			  false recule)
* Retour : non
***********************************************************************************/
void roueGauche(bool direction, uint8_t rapport){
	if (!direction) {
		PORTD |= 0x01 << PD2;
	}
	else {
		PORTD &= ~(0x01 << PD2);
	}
	rapport= (rapport)*0xff/100;
	OCR1B = rapport;
}

/**********************************************************************************
* Methode : roueDroite (bool direction, uint8_t rapport )
* Role : Ajuster la vitesse et la direction de la roue droite 
* Parametre : - rapport, correspond au pourcentge de la vitesse max du robot(100%, 50%...)
			  - direction, correspond a l direction du robot (true avance,
			  false recule)
* Retour : non
***********************************************************************************/
void roueDroite(bool direction,uint8_t rapport){
	if (!direction) {
		PORTD |= 1 << PD7;
	}
	else {
		PORTD &= ~(1 << PD7);
	}
	rapport= (rapport+7)*255/100;
	OCR1A = rapport;
}

// Deplacement en ligne droite

/**********************************************************************************
* Methode : avancer ( uint8_t rapport )
* Role : Avancer en ligne droite a la vitesse ( rapport / 100 * 255)
* Parametre : rapport, correspond au pourcentge de la vitesse max du robot (100%, 50%...)
* Retour : non
***********************************************************************************/
void avancer(uint8_t rapport) {

	roueDroite(true, rapport);
	roueGauche(true, rapport);

}

/**********************************************************************************
* Methode : reculer ( uint8_t rapport )
* Role : Reculer en ligne droite a la vitesse ( rapport / 100 * 255%)
* Parametre : rapport, correspond au pourcentge de la vitesse max du robot (100%, 50%...)
* Retour : non
***********************************************************************************/
void reculer(uint8_t rapport){
	roueDroite(false, rapport);
	roueGauche(false, rapport);
}

/**********************************************************************************
* Methode : arreter ()
* Role : Arreter les deux moteurs
* Parametre : non
* Retour : non
***********************************************************************************/
void arreter(){
	OCR1A = 0;
	OCR1B = 0;
}

// Rotation 

/**********************************************************************************
* Methode : TournerADroite ()
* Role : Virage du robot de 90 degres a droite
* Parametre : non
* Retour : non
***********************************************************************************/
void tournerADroite(){
    // Tourner a droite : bloquer la roue droite et faire tourner vers l'avant la roue gauche
	OCR1A = 0;
	PORTD &= ~(1 << PD2);
	OCR1B = 255; // A determiner avec le robot
	_delay_ms(800); // A determiner avec le robot

}


/**********************************************************************************
* Methode : TournerAGauche ()
* Role : Virage du robot de 90 degres a gauche
* Parametre : non
* Retour : non
***********************************************************************************/
void tournerAGauche(){
	// Tourner a gauche : bloquer la roue gauche et faire tourner vers l'avant la roue droite
	OCR1B = 0;
	PORTD &= ~(1 << PD7);
	OCR1A= 255; // A determiner avec le robot
	_delay_ms(800); // A determiner avec le robot


	
}

/**********************************************************************************
* Methode : ajustementADroite ()
* Role : Ajustement de la direction du robot (decalage sur la gauche)
* Parametre : non
* Retour : non
***********************************************************************************/

void ajustementADroite (){

	roueDroite(true, 35);
	roueGauche(true, 60);

}

/**********************************************************************************
* Methode : ajustementAGauche ()
* Role : Ajustement de la direction du robot (decalage sur la droite)
* Parametre : non
* Retour : non
***********************************************************************************/

void ajustementAGauche(){

	roueDroite(true, 60);
	roueGauche(true, 35);

}


/**********************************************************************************
* Methode : ajustementReculerAGauche ()
* Role : Ajustement de la direction du robot (decalage sur la gauche)
* Parametre : non
* Retour : non
***********************************************************************************/
void ajustementReculerAGauche(){
	roueDroite(false, 50);
	roueGauche(false, 75);
}


/**********************************************************************************
* Methode : ajustementReculerADroite ()
* Role : Ajustement de la direction du robot (decalage sur la droite)
* Parametre : non
* Retour : non
***********************************************************************************/
void ajustementReculerADroite(){
	roueDroite(false, 50);
	roueGauche(false, 25);
}