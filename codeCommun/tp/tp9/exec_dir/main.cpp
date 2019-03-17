#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include "del.h"
#include "piezo.h"
#include "util.h"

int main() {
	//PINS 5 and 3 for piezo
	PIEZO_INIT(DDD4, DDD2); //First pin is the PWM output. Second pin passed is the ground pin.
   	uint8_t count = 45;
   	for(;;){
		PLAY_NOTE(count);
		count++;
		if(count==81){
			count=45;
		}
	}
    return 0;
}

//source:
//https://web.archive.org/web/20111128004851/http://aquaticus.info/pwm-music
