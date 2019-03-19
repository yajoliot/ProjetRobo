#include "pwm.h"

void initPWM(){ 

	// mise à un des sorties OC0A et OC0B sur comparaison
	// réussie en mode PWM 8 bits, phase correcte
	// et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6
	// page 177 de la description technique du ATmega324PA)

	//(PCINT11/OC0A/AIN1) PB3 (identified as 4 on the board)
	//(PCINT12/OC0B/SS) PB4 (identified as 5 on the board)

	TCNT0 = 0;

	TCCR0A |= (1 << COM0A1) | (1<< COM0B1);
	TCCR0A |= (1 << WGM00);
	TCCR0B |= (1 << CS01);

}


// Ajustement de la vitesse des roues



void roueGauche(bool direction, uint8_t rapport){
	if (!direction) {
		PORTB |= 0x01 << PB2;
	}
	else {
		PORTB &= ~(0x01 << PB2);
	}
	rapport= (rapport)*0xff/100;
	OCR0B = rapport;
}


void roueDroite(bool direction,uint8_t rapport){
	if (!direction) {
		PORTB |= 1 << PB5;
	}
	else {
		PORTB &= ~(1 << PB5);
	}
	rapport= (rapport+7>100?100:rapport+7)*255/100;
	OCR0A = rapport;
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
	OCR0A = 0;
	OCR0B = 0;
}

// Rotation 

void tournerADroite(){
    // Tourner a droite : bloquer la roue droite et faire tourner vers l'avant la roue gauche
	OCR0A = 0;
	PORTD &= ~(1 << PB2);
	OCR0B = 255; // A determiner avec le robot
	_delay_ms(800); // A determiner avec le robot

}


void tournerAGauche(){
	// Tourner a gauche : bloquer la roue gauche et faire tourner vers l'avant la roue droite
	OCR0B = 0;
	PORTD &= ~(1 << PB5);
	OCR0A= 255; // A determiner avec le robot
	_delay_ms(800); // A determiner avec le robot
	
}

