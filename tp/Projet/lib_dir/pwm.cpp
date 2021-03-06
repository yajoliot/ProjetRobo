#include "pwm.h"


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
uint8_t PWM::getVitesseDefault(){
	return VITESSE_DEFAULT;
}

uint8_t PWM::getVitesseTournage(){
	return vitesseTournage;
}





// Ajustement de la vitesse des roues



void PWM::roueGauche(bool direction, uint8_t rapport){
	if (!direction) {
		PORTB |= 0x01 << PB2;
	}
	else {
		PORTB &= ~(0x01 << PB2);
	}
	OCR0B = rapport;

	this->rapportGauche = OCR0B;
	this->directionGauche = direction;
}


void PWM::roueDroite(bool direction,uint8_t rapport){
	if (!direction) {
		PORTB |= 1 << PB5;
	}
	else {
		PORTB &= ~(1 << PB5);
	}
	OCR0A = rapport;

	this->rapportDroite = OCR0A;
	this->directionDroite = direction;
}


void PWM::tournantGauche(uint8_t &rapport, uint8_t valueMap){
	rapport = getVitesseDefault();
	if(valueMap == 7 || valueMap == 3|| valueMap == 15){
		roueGauche(true, rapport/2);
		roueDroite(true, rapport);
	}
	else if (valueMap == 1) {
		roueGauche(true, rapport/3);
		roueDroite(true, rapport);
	}
	else if (valueMap == 0) {
		roueGauche(true, rapport/5);
		roueDroite(true, rapport);
	}

}

void PWM::tournantDroite(uint8_t &rapport, uint8_t valueMap){
	rapport = getVitesseDefault();
	if(valueMap == 28 || valueMap == 24 || valueMap == 30){
		roueDroite(true, rapport/2);
		roueGauche(true, rapport);
	}
	else if (valueMap == 16) {
		roueDroite(true, rapport/3);
		roueGauche(true, rapport);
	}
	else if (valueMap == 0) {
		roueDroite(true, rapport/5);
		roueGauche(true, rapport);
	}

}

void PWM::boite(uint8_t &rapport, uint8_t valueMap){
	if(valueMap == 31 || valueMap == 0){
		rapport = VITESSE_DEFAULT; 
		avancer(rapport);
	}
	else if(valueMap == 1 || valueMap == 3){
		if(rapport == 0){
		}else
			rapport = rapport - 2 < 85 ? rapport : rapport - 2;
		roueGauche(true, VITESSE_DEFAULT);
		roueDroite(true, rapport);
	}
	else if(valueMap == 16 || valueMap == 24){
		if(rapport == 0){
		}else
			rapport = rapport - 2 < 85 ? rapport : rapport - 2;
		roueGauche(true, rapport);
		roueDroite(true, VITESSE_DEFAULT);
	}
	
}

// Deplacement en ligne droite


void PWM::avancer(uint8_t rapport) {

	rapport - AJUSTEMENT_DROITE > 0 ? roueDroite(true, rapport - AJUSTEMENT_DROITE) : roueGauche(true, rapport);
	//ajustement du pwm pour avancer en ligne droite
	rapport - AJUSTEMENT_GAUCHE > 0 ? roueGauche(true, rapport - AJUSTEMENT_GAUCHE) : roueGauche(true, rapport);
	

}

void PWM::avancementAjuste(uint8_t &rapport, uint8_t valueMap) {
	// roueDroite(true, VITESSE_MAX);
	// roueGauche(true, VITESSE_MAX);
	
	if(valueMap == 4 || valueMap == 31){
		rapport = VITESSE_DEFAULT;
		this->avancer(rapport);
	} else if(valueMap == 6 || valueMap == 2 || valueMap == 3 || valueMap == 1){
		rapport = rapport - 1 > VITESSE_DEFAULT - DIMINUTION_MINIMALE ? rapport - 1: rapport;
		roueDroite(true, VITESSE_DEFAULT);
		roueGauche(true, rapport);

	} else if(valueMap == 12 || valueMap == 8 || valueMap == 24 || valueMap == 16) {
		rapport = rapport - 1 > VITESSE_DEFAULT-DIMINUTION_MINIMALE ? rapport - 1: rapport;
		roueDroite(true, rapport);
		roueGauche(true, VITESSE_DEFAULT);

	}
}

void PWM::ralentissementGauche(uint8_t &rapport, uint8_t valueMap) {
	// roueDroite(true, VITESSE_MAX);
	// roueGauche(true, VITESSE_MAX);
	
	if(valueMap == 4 || valueMap == 31){
		rapport = VITESSE_DEFAULT;
		this->avancer(rapport);
	} else if(valueMap == 6 || valueMap == 2 || valueMap == 3 || valueMap == 1){
		rapport = rapport - 6 > VITESSE_DEFAULT-DIMINUTION_REDUCTION ? rapport - 6: rapport;
		roueDroite(true, rapport + 25);
		roueGauche(true, rapport);

	} else if(valueMap == 12 || valueMap == 8 || valueMap == 24 || valueMap == 16) {
		rapport = rapport - 6 > VITESSE_DEFAULT-DIMINUTION_REDUCTION ? rapport - 6: rapport;
		roueDroite(true, rapport);
		roueGauche(true, rapport + 25);

	}
}





void PWM::reculer(uint8_t rapport){
	roueDroite(false, rapport);
	roueGauche(false, rapport);
}


void PWM::arreter(){
	OCR0A = 0;
	OCR0B = 0;

	rapportGauche = OCR0B;
	rapportDroite = OCR0A;
}

// Rotation 

void PWM::tournerADroite(){
    // Tourner a droite : bloquer la roue droite et faire tourner vers l'avant la roue gauche
	OCR0A = 0;
	PORTD &= ~(1 << PB2);
	OCR0B = 255; // A determiner avec le robot
	_delay_ms(800); // A determiner avec le robot

	rapportGauche = OCR0B;
	rapportDroite = OCR0A;

}




void PWM::tournerAGauche(){
	// Tourner a gauche : bloquer la roue gauche et faire tourner vers l'avant la roue droite
	OCR0B = 0;
	PORTD &= ~(1 << PB5);
	OCR0A= 255; // A determiner avec le robot
	_delay_ms(900); // A determiner avec le robot

	rapportGauche = OCR0B;
	rapportDroite = OCR0A;

}

void PWM::tourner90Droite(uint8_t rapport){

	roueGauche(true, 0);
	roueDroite(false, rapport);
	_delay_ms(1500);
	arreter();
}

void PWM::tourner90Gauche(uint8_t rapport){

	roueGauche(false, rapport);
	roueDroite(true, 0);
	_delay_ms(1500);
	arreter();
}

//section1
void PWM::avancerTimer(uint8_t valeur, uint32_t timer){
	arreter();
    _delay_ms(300);
	startMinuterie(0xFFFF);
	while(TCNT1 < valeur * timer)
        avancer(getVitesseDefault());
    stopMinuterie();
    resetMinuterie();
}

void PWM::tourner90Precis(uint8_t direc, uint8_t rapport){
	arreter();
    _delay_ms(300);
	if( direc == 0 )
        tourner90Gauche(rapport);
	else
		tourner90Droite(rapport);
	
}

void PWM::rotateGauche(uint8_t rapport){
	roueGauche(false, rapport);
	roueDroite(true, rapport);
}

void PWM::rotateDroite(uint8_t rapport){
	roueGauche(true, rapport);
	roueDroite(false, rapport);
}



