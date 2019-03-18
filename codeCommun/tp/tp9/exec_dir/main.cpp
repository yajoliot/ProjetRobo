#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include "pwm.h"

//SETUP is...

// PD2 + PD4 (identified as 1 and 5 on the board) for roueGauche
// PD5 + PD7 (identified as 6 and 8 on the board) for roueDroite

// PD2: direction 
// PD4: pwm

// PD5: pwm
// PD7: direction

// Plug these so that the direction pin is connected to the D pin on the pont H for either roue and the pwm pin to the E pin on either roue 

int main() {
  DDRB = 0xFF;
  initPWM();
  for(;;){
    avancer(50);
    _delay_ms(3000);
    arreter();
    tournerADroite();
    tournerAGauche();
   		//these functions cover nearly everything. reculer() wasn't called but same idea as avancer().
  }
  return 0;
}