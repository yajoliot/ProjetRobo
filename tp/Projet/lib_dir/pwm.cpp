#include <PWM.h>


PWM::PWM(){
	PWM::initPWM();
}
void PWM::initPWM(){
	
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

// Getters
uint8_t PWM::getRapportGauche(){
	return rapportGauche;
}

uint8_t PWM::getRapportDroite(){
    return rapportDroite;
}

uint8_t PWM::getDirectionGauche(){
	return directionGauche;
}

uint8_t PWM::getDirectionDroite(){
	return directionDroite;
}




// Ajustement de la vitesse des roues



void PWM::roueGauche(bool direction, uint8_t rapport){
	if (!direction) {
		PORTD |= 0x01 << PD2;
	}
	else {
		PORTD &= ~(0x01 << PD2);
	}
	rapport= (rapport)*0xff/100;
	OCR1B = rapport;

	this->rapportGauche = OCR1B;
	this->directionGauche = direction;
}


void PWM::roueDroite(bool direction,uint8_t rapport){
	if (!direction) {
		PORTD |= 1 << PD7;
	}
	else {
		PORTD &= ~(1 << PD7);
	}
	rapport= (rapport+7)*255/100;
	OCR1A = rapport;

	this->rapportDroite = OCR1A;
	this->directionDroite = direction;
}

// Deplacement en ligne droite


void PWM::avancer(uint8_t rapport) {

	roueDroite(true, rapport);
	roueGauche(true, rapport);

	this->rapportGauche = rapport;
	this->rapportDroite = rapport;

}


void PWM::reculer(uint8_t rapport){
	roueDroite(false, rapport);
	roueGauche(false, rapport);

	this->rapportGauche = rapport;
	this->rapportDroite = rapport;
    this->directionGauche = 0x00;
	this->directionDroite = 0x00;
}


void PWM::arreter(){
	OCR1A = 0;
	OCR1B = 0;

	rapportGauche = OCR1B;
	rapportDroite = OCR1A;
}

// Rotation 

void PWM::tournerADroite(){
    // Tourner a droite : bloquer la roue droite et faire tourner vers l'avant la roue gauche
	OCR1A = 0;
	PORTD &= ~(1 << PD2);
	OCR1B = 255; // A determiner avec le robot
	_delay_ms(800); // A determiner avec le robot

	rapportGauche = OCR1B;
	rapportDroite = OCR1A;

}


void PWM::tournerAGauche(){
	// Tourner a gauche : bloquer la roue gauche et faire tourner vers l'avant la roue droite
	OCR1B = 0;
	PORTD &= ~(1 << PD7);
	OCR1A= 255; // A determiner avec le robot
	_delay_ms(800); // A determiner avec le robot

	rapportGauche = OCR1B;
	rapportDroite = OCR1A;

}

