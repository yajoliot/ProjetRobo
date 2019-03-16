#ifndef LIB_DIR_CONTROLPWM_H
#define LIB_DIR_CONTROLPWM_H

#include <avr/io.h> 
#include <util/delay.h>

void initPWM();
void roueGauche(bool direction, uint8_t rapport);
void roueDroite(bool direction,uint8_t rapport);
void avancer(uint8_t rapport);
void reculer(uint8_t rapport);
void arreter();
void tournerADroite();
void tournerAGauche();
void ajustementADroite ();
void ajustementAGauche();
void ajustementReculerAGauche();
void ajustementReculerADroite();

#endif 
