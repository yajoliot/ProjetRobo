#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include "couleurDel.h"
#include "notes.h"
#include "piezo.h"

//general defines
#define _SIZEOF(x) sizeof(x)/sizeof(x[0])

int main() {
	//PINS 5 and 3 for piezo
	PIEZO_INIT(PD4);
	DDRD |= _BV(DDD2); //have to define yourself which other pin the piezo will be taking as a ground
   	uint8_t count = 0;
   	for(;;){
		ICR1H = (note[count] >> 8); //first set the high byte
		ICR1L = note[count];        //now the low byte
		_delay_ms(500);
		count++;
		count=count%(_SIZEOF(note));
	}
    return 0;
}

//source:
//https://web.archive.org/web/20111128004851/http://aquaticus.info/pwm-music
