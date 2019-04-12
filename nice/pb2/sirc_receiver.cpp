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

#define CYCLES_PER_T 0x18 ////For one T worth of time -> 600 us / 25 us = 24 cycles. 0x18 = 24 cycles.

void sender_func(){
    //DEBUG_FUNCTION_CALL((uint8_t*)"ISR(TIMER1_OVF_vect)");
    count++;
    //DEBUG_PARAMETER_VALUE((uint8_t*)"count",(void*)&count);

    //Need to put this stop logic here otherwise ISR is too quick!
    if(count>=CYCLES_PER_T){
        //Reset count to 0
        count = 0x00;

        //Stop the PWM
        SIRC_SENDER::disableSender();

        //Set the boolean for the ISR setting function
        SIRC_SENDER::reach_end_T = true;
    }
}

ISR(TIMER1_OVF_vect)
{
	switch(turn){
		case SENDER: sender_func();
		case RECEIVER: timer_end = true;
	}
}

//PCINT1_vect for the PORTB
volatile uint8_t pna4602m = 0x00;
ISR(PCINT3_vect){
	pna4602m ^= _BV(0);
	//high edge
	if(pna4602m & _BV(0)){
		high_edge = true;
		low_edge = false;
	}//low edge
	else{
		high_edge = false;
		low_edge = true;
	}
	DEBUG_INFO((uint8_t*)"!!!!!!!!!! got something");
}

void startMinuterie();

uint8_t detectHeader(){
	if(low_edge){
		startMinuterie();
		OCR1A = 20000;//2500us
		while(low_edge){if(timer_end){
			//successful
			return 1;
		}}
		//error
		return 0;
	}else{
		//abort early i guess
		return 0;
	}
}

uint8_t detectOneCycle(){
	if(low_edge){
		startMinuterie();
		OCR1A = 4800; //1200us
		while(low_edge){if(timer_end){
			timer_end = false;
			while(low_edge){if(timer_end){
				return 2;
			}}
			return 1;
		}}
		return 0;
	}else{
		return 0;
	}
}

uint8_t detectBits(uint8_t size){
	uint8_t result = 0x00;
	for(uint8_t i=0 ; i<size ; i++){
		uint8_t ret_val = detectOneCycle();
		if(ret_val == 2){
			//1
			result |= _BV(0) << (size-i-1);
		}else if(ret_val == 1){
			//0
			result &= ~(_BV(0)) << (size-i-1);
		}else{
			//error. propagate error.
			return 0;
		}
	}
	return result;
}

uint16_t detect(){
	uint8_t command = 0x00;
	uint8_t address = 0x00;
	uint8_t detect_res = detectHeader();
	if(detect_res){
		command = detectBits(COMMAND_SIZE);
		address = detectBits(ADDRESS_SIZE);
	}else if(detect_res == 0){
		//error i guess
		return 0x0000;
	}
	uint16_t result = address; result = result << 8;
	result |= command;
	return result;
}


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

	for(;!a;){
		unitTest_PNA4602M();
	}
	abort();
}

void unitTest_PNA4602M(){
	//if ISR is triggered
	// uint8_t tmp = PIND & 0x40;
	// DEBUG_PARAMETER_VALUE((uint8_t*)"//////////////// PORTD6",&tmp);

	uint16_t nice = detect();
	if(nice == 0){
		DEBUG_INFO((uint8_t*)"nice");
		return ;
	}else{
		//result!
		uint8_t res = (uint8_t)nice;
		DEBUG_PARAMETER_VALUE((uint8_t*)"command result", &res);
	}
	// if(edge_detected){
	// 	PIEZO_INIT(DDD5, DDD7);
	//    	for(;;){
	// 		PLAY_NOTE(key);
	// 		_delay_ms(30);
	// 		key++;
	// 		if(key==81){
	// 			key=45;
	// 		}
	// 	}
	// 	//reset
	// 	edge_detected = false;	
	// }
}

