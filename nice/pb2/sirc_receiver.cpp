#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include "usart.h"
#include "debug.h"
#include "sirc_sender.h"
#include "piezo.h"
#include "sirc_constantes.h"
#include "ISR.h"

//PCINT1_vect for the PORTB
volatile uint8_t pna4602m = 0x00;
ISR(PCINT3_vect){
	pna4602m ^= _BV(0);
	//high edge
	if(pna4602m & _BV(0)){
		high_edge = true;
	}//low edge
	else{
		low_edge = true;
	}
	DEBUG_INFO((uint8_t*)"!!!!!!!!!! got something");
}

void detectHeader(){
	if(low_edge){
		startMinuterie();
		OCR1A = 20000//2500us
	}
}

void detect


void setup(){
	//Enable that PINB6-7 will trigger an ISR event on any change (edge change i guess)
	PCMSK3 |= _BV(PCINT30);//PCMSK1 |= _BV(PCINT14) | _BV(PCINT15);
	//Enable the whole PINB to trigger the interrupt
	PCICR |= _BV(PCIE3);//_BV(PCIE1);
}

void startMinuterie(){
	TCCR1B |= _BV(CS12) | _BV(CS10);
    TIMSK1 |= _BV(TOIE1);
}

volatile bool a = false;
ISR(INT0_vect, ISR_NAKED){
	cli();
	DEBUG_INFO((uint8_t*)"we in");
	a = true;
}

void unitTest_PNA4602M();

void clearTimer1Registers(){
	TCCR1A &= ~(_BV(WGM11)) & ~(_BV(WGM10));
	TCCR1A &= ~(_BV(COM1A1)) & ~(_BV(COM1A0)) & ~(_BV(COM1B1)) & ~(_BV(COM1B0));
	TCCR1B &= ~(_BV(WGM13)) & ~(_BV(WGM12));
	TCCR1B &= ~(_BV(CS12)) & ~(_BV(CS11)) & ~(_BV(CS10));
	TCNT1 = 0;
	TIMSK1 &= ~(_BV(TOIE1));
}

uint8_t key = 45;

int main(){
	//setup ports
	DDRD = 0xBB; //1011 1011
	DDRA = 0xFF;
	DDRB = 0x3F; //2 last one in input

	//allow for pcint interrupt
	setup();

    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC01);
    sei();

	// for(;!a;){
	unitTest_PNA4602M();
	// }
	abort();
}

void unitTest_PNA4602M(){
	SIRC_SENDER sirc;
	sirc.transmit(0x01, 0x02);
	//if ISR is triggered
	// uint8_t tmp = PIND & 0x40;
	// DEBUG_PARAMETER_VALUE((uint8_t*)"//////////////// PORTD6",&tmp);
	if(edge_detected){
		PIEZO_INIT(DDD5, DDD7);
	   	for(;;){
			PLAY_NOTE(key);
			_delay_ms(30);
			key++;
			if(key==81){
				key=45;
			}
		}
		//reset
		edge_detected = false;	
	}
}