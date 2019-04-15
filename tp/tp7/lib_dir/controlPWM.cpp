#include <controlPWM.h>

void initPWM(){ 
	
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


void avancer(uint8_t rapport) {

	roueDroite(true, rapport);
	roueGauche(true, rapport);

}


void reculer(uint8_t rapport){
	roueDroite(false, rapport);
	roueGauche(false, rapport);
}


void arreter(){
	OCR1A = 0;
	OCR1B = 0;
}

// Rotation 

void tournerADroite(){
    // Tourner a droite : bloquer la roue droite et faire tourner vers l'avant la roue gauche
	OCR1A = 0;
	PORTD &= ~(1 << PD2);
	OCR1B = 255; // A determiner avec le robot
	_delay_ms(800); // A determiner avec le robot

}


void tournerAGauche(){
	// Tourner a gauche : bloquer la roue gauche et faire tourner vers l'avant la roue droite
	OCR1B = 0;
	PORTD &= ~(1 << PD7);
	OCR1A= 255; // A determiner avec le robot
	_delay_ms(800); // A determiner avec le robot


	
}

