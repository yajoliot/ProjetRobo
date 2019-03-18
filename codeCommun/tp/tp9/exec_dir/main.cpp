#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include "pwm.h"
#include "piezo.h"

//SETUP is...



// MOTOR PWM

// PD2 + PD4 (identified as 1 and 5 on the board) for roueGauche
// PD5 + PD7 (identified as 6 and 8 on the board) for roueDroite

// PD2: direction 
// PD4: pwm

// PD5: pwm
// PD7: direction

// Plug these so that the direction pin is connected to the D pin on the pont H for either roue and the pwm pin to the E pin on either roue 



// PIEZO PWM

// PD3 or PD4 create the PWM (or... Pin(5) and Pin(6) as identified on the board)

// Choose any of the surrounding pins to set to ground and pass it to the constructor along with the PD3 or PD4

// Plug the piezo in any way but on those pins you've passed to the constructor

int main() {
  DDRB = 0xFF;
  initPWM();
  PIEZO_INIT(DDD4, DDD2);
  uint8_t count;
  for(;;){
    
    //MOTOR PWM test
    avancer(50);
    _delay_ms(3000);
    arreter();
    tournerADroite();
    tournerAGauche();
    arreter();
	//these functions cover nearly everything. reculer() wasn't called but same idea as avancer().

	//PIEZO test
	count=45;
    for(;count<81;count++){
		PLAY_NOTE(count);
	}
  }
  return 0;
}