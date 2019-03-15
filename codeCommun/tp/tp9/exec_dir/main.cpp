#include <avr/io.h>
#include "couleurDel.h"

int main() {
    DDRD |= _BV(DDD4); //OC1B as output
    TCCR1A |= _BV(COM1B1);  //Clear OC1A/OC1B on compare match
	TCCR1B |= _BV(WGM13) 	//mode 8, PWM, Phase and Frequency Correct (TOP value is ICR1)
   		   |  _BV(CS11); 	//prescaler(8)
   	const uint16_t note[] = { 1911, 1703, 1517 , 1432 , 1276 , 1136 , 1012 };
   	OCR1B = 100;
   	uint8_t count = 0;
   	for(;;){
		ICR1H = (note[count] >> 8); //first set the high byte
		ICR1L = note[count];        //now the low byte
		_delay_ms(2000);
		count++;
		count=count%7;
	}
    return 0;
}
