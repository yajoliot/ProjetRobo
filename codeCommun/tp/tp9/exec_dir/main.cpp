#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include "couleurDel.h"
#include "piezo.h"

//general defines
#define _SIZEOF(x) sizeof(x)/sizeof(x[0])

int main() {
	//PINS 5 and 3 for piezo
	PIEZO_INIT(PD4);
	DDRD |= _BV(DDD2); //have to define yourself which other pin the piezo will be taking as a ground
   	uint8_t count = 45;
   	for(;;){
		PLAY_NOTE(count);
		count++;
		if(count==81){
			count=count%81;
			count+=45;
		}
	}
    return 0;
}

//source:
//https://web.archive.org/web/20111128004851/http://aquaticus.info/pwm-music
