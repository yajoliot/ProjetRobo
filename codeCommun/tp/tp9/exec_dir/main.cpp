#include <avr/io.h>
#include "couleurDel.h"
#include "notes.h"

//custom variadic function defines for piezo
#define VARIADIC_TWO(a, b, ...) (a), (b)
#define PIEZO_INIT(...) piezo_init(JUST3(__VA_ARGS__, 100))

//general defines
#define _SIZEOF(x) sizeof(x)/sizeof(x[0])

int main() {
	//PINS 5 and 3 for piezo
	DDRD |= _BV(DDD2); 
	DDRD |= _BV(DDD4); //OC1B as output
	TCCR1A |= _BV(COM1B1);  //Clear OC1A/OC1B on compare match
	TCCR1B |= _BV(WGM13) 	//mode 8, PWM, Phase and Frequency Correct (TOP value is ICR1)
   		   |  _BV(CS11); 	//prescaler(8)
   	uint8_t temp = 100;
   	uint8_t count = 0;
   	for(;;){
   		OCR1B = temp;
   		if(count%2==0){
   			temp-=10;
   			if(temp==0)
   				temp=100;
   		}
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
