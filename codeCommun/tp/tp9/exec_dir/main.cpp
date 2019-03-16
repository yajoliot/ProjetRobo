#include <avr/io.h>
#include "couleurDel.h"
#include "notes.h"
#include "piezo.h"

//custom variadic function defines for piezo
//source:https://stackoverflow.com/a/9180314
#define VARIADIC_TWO(a, b, ...) (a), (b)
#define PIEZO_INIT(...) piezo_init(VARIADIC_TWO(__VA_ARGS__, 100))

//general defines
#define _SIZEOF(x) sizeof(x)/sizeof(x[0])

int main() {
	//PINS 5 and 3 for piezo
	PIEZO_INIT(PD4);
	DDRD |= _BV(DDD2);
   	uint8_t count = 0;
   	for(;;){
		ICR1H = (note[count] >> 8); //first set the high byte
		ICR1L = note[count];        //now the low byte
		_delay_ms(2000);
		count++;
		count=count%(_SIZEOF(note));
	}
    return 0;
}

//source:
//https://web.archive.org/web/20111128004851/http://aquaticus.info/pwm-music
