#include <PWM.h>


PWM::PWM(){
	PWM::initPWM();
}


void PWM::initPWM(){ 

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
		PORTB |= 0x01 << PB2;
	}
	else {
		PORTB &= ~(0x01 << PB2);
	}
	rapport= ((rapport)/0xff)*100;
	OCR0B = rapport;

	this->rapportGauche = OCR1B;
	this->directionGauche = direction;
}


void PWM::roueDroite(bool direction,uint8_t rapport){
	if (!direction) {
		PORTB |= 1 << PB5;
	}
	else {
		PORTB &= ~(1 << PB5);
	}
	rapport= ((rapport)/255)*100;
	OCR0A = rapport;

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
	PORTD &= ~(1 << PB2);
	OCR1B = 255; // A determiner avec le robot
	_delay_ms(800); // A determiner avec le robot

	rapportGauche = OCR1B;
	rapportDroite = OCR1A;

}


void PWM::tournerAGauche(){
	// Tourner a gauche : bloquer la roue gauche et faire tourner vers l'avant la roue droite
	OCR1B = 0;
	PORTD &= ~(1 << PB5);
	OCR1A= 255; // A determiner avec le robot
	_delay_ms(800); // A determiner avec le robot

	rapportGauche = OCR1B;
	rapportDroite = OCR1A;

}

