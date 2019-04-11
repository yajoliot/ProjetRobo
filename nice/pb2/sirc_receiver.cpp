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

// // ISR(TIMER1_OVF_vect){
// // 	timer_ended = true;
// // }


// ISR(TIMER2_OVF_vect){
//     overflow_count++;
//     if(overflow_count>=OVERFLOW_COUNT_TO_45_MS){
//         overflow_count = 0x0000;
//         SIRC_SENDER::end45msTimer();
//         reach_end_45_ms = TRUE;
//     }
// }

// // ISR(TIMER1_OVF_vect){
// // 	count_++;
// // }

// //PCINT1_vect for the PORTB
// ISR(PCINT3_vect){
// 	//Sensor will come out on PINB6-7. Rising edge stimulation. If need for falling edge do: !(PINB & 0xC0)
// 	edge_detected = true;
// 	DEBUG_INFO((uint8_t*)"got something");
// }

// void sender_func(){
//     //DEBUG_FUNCTION_CALL((uint8_t*)"ISR(TIMER1_OVF_vect)");
//     count++;
//     //DEBUG_PARAMETER_VALUE((uint8_t*)"count",(void*)&count);

//     //Need to put this stop logic here otherwise ISR is too quick!
//     if(count>=CYCLES_PER_T){
//         //Reset count to 0
//         count = 0x00;

//         //Stop the PWM
//         SIRC_SENDER::disableSender();

//         //Set the boolean for the ISR setting function
//         reach_end_T = TRUE;
//     }
// }

// ISR(TIMER1_OVF_vect)
// {
// 	switch(turn){
// 		case SENDER: sender_func();
// 		case RECEIVER: count_++;
// 	}
// }

void setup(){
	//Enable that PINB6-7 will trigger an ISR event on any change (edge change i guess)
	PCMSK1 |= _BV(PCINT30);//_BV(PCINT14) | _BV(PCINT15);
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

	//unit test

 //    PIEZO_INIT(DDD5, DDD7);
 //   	for(;;){
	// 	PLAY_NOTE(key);
	// 	_delay_ms(30);
	// 	key++;
	// 	if(key==81){
	// 		key=45;
	// 	}
	// }

	for(;!a;){
		unitTest_PNA4602M();
	}	
}

void unitTest_PNA4602M(){
	SIRC_SENDER sirc;
	sirc.transmit(0x01, 0x02);
	//if ISR is triggered
	uint8_t tmp = PIND & 0x40;
	DEBUG_PARAMETER_VALUE((uint8_t*)"//////////////// PORTD6",&tmp);
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