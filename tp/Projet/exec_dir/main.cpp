#ifndef F_CPU
#define F_CPU 8000000UL
#endif
 
#include <util/atomic.h>
#include <util/delay.h>
#include "del.h"
#include "debug.h"

#define HIGH_MODE 0x01
#define LOW_MODE 0x00

#define COMMAND_SIZE 0x07
#define ADDRESS_SIZE 0x05

//For one T worth of time -> 600 us / 25 us = 24 cycles
#define CYCLES_PER_T 0x18 //24

#define FALSE 0x00
#define TRUE 0x01

void transmitOneCycle(uint8_t mode);
void transmitHighBit();
void transmitLowBit();
void transmitHeader();
void transmitCommand(uint8_t command_);
void transmitAddress(uint8_t address_);
void enableSIRC();
void disableSIRC();
void setupSIRC();

volatile uint8_t count = 0x00;
volatile uint8_t reach_end_T = FALSE;

ISR(TIMER1_OVF_vect)
{
    //DEBUG_FUNCTION_CALL((uint8_t*)"ISR(TIMER1_OVF_vect)");
    count++;
    //DEBUG_PARAMETER_VALUE((uint8_t*)"count",(void*)&count);

    //Need to put this stop logic here otherwise ISR is too quick!
    if(count>=CYCLES_PER_T){
        //Reset count to 0
        count = 0x00;

        //Stop the PWM
        disableSIRC();

        //Set the boolean for the ISR setting function
        reach_end_T = TRUE;
    }
}

int main() {
    DEBUG_INFO((uint8_t*)"START OF PROGRAM");

//Port Setup

    DDRD = 0xFF;
     
//PWM Setup

    setupSIRC(); //enable/disable as you need

//Global interrupt setup

    sei();

//Create a random command

    //Transmit header
    
    DEBUG_FUNCTION_CALL((uint8_t*)"transmitHeader()");
    transmitHeader();

    //Transmit command

    //dummy command
    uint8_t command = 0x01;
    DEBUG_FUNCTION_CALL((uint8_t*)"transmitCommand()");
    transmitCommand(command);

    //Transmit address

    //dummy address
    uint8_t address = 0x02;
    DEBUG_FUNCTION_CALL((uint8_t*)"transmitAddress()");
    transmitAddress(address);

    DEBUG_INFO((uint8_t*)"END OF PROGRAM");
}

void setupSIRC(){

    //Set the registers : CTC mode, Toggle on Compare, No prescaler
    TCCR1A |= _BV(WGM12) | _BV(COM1A0);
    //TCCR1B |= _BV(CS10);
    
    //Set the frequency : 40 kHz
    //The following value is what we will use to set the "one"
    OCR1A = 0x63;

}

void enableSIRC(){
    //Non 0 value for the prescaler enables the timer
    TCCR1B |= _BV(CS10);
    //Enable timer interrupt 
    TIMSK1 |= _BV(TOIE1); 
}

void disableSIRC(){
    //Sets the only prescaler value we touched to 0
    TCCR1B &= ~(_BV(CS10));
    //Disable timer interrupt
    TIMSK1 &= ~(_BV(TOIE1));
    // cli();
}

void transmitOneCycle(uint8_t mode){
    // DEBUG_FUNCTION_CALL((uint8_t*)"transmitOneCycle()");
    //Set the PWM
    enableSIRC();
    //Wait until ISR ends itself the 24 cycles for 1 T
    while(!reach_end_T){}
    reach_end_T = FALSE;

    // uint8_t tmp = TCCR1B;
    // DEBUG_PARAMETER_VALUE((uint8_t*)"TCCR1B",&tmp);
    // DEBUG_FUNCTION_EXIT();
}

void transmitHighBit(){
    DEBUG_FUNCTION_CALL((uint8_t*)"transmitHighBit()");
    //Two Cycles @ Hi
    for(uint8_t i=0x00 ; i<0x02; i++){
        transmitOneCycle(HIGH_MODE);
    }
    //One Cycle @ Lo
    transmitOneCycle(LOW_MODE);
}

void transmitLowBit(){
    DEBUG_FUNCTION_CALL((uint8_t*)"transmitLowBit()");
    //One Cycle @ Hi
    transmitOneCycle(HIGH_MODE);
    //One Cycle @ Lo
    transmitOneCycle(LOW_MODE);
}

void transmitHeader(){
    //Four Cycles @ Hi
    for(uint8_t i=0x00 ; i<0x04 ; i++){
        transmitOneCycle(HIGH_MODE);
    }
    //One Cycle @ Lo
    transmitOneCycle(LOW_MODE);
}


void transmitBits(uint8_t command_, uint8_t size_){
    DEBUG_FUNCTION_CALL((uint8_t*)"transmitBits(uint8_t command_, uint8_t size_)");
    for(uint8_t i=0x00 ; i<size_ ; i++){
        //if the bit @ the ith position is 1
        uint8_t bitmask = _BV(i);
        DEBUG_PARAMETER_VALUE((uint8_t*)"_BV(i)", &bitmask);
        if(command_ & bitmask){
            //then transmit a high bit
            transmitHighBit();
        }else{
            transmitLowBit();
        }
    }
}

void transmitCommand(uint8_t command_){
    DEBUG_FUNCTION_CALL((uint8_t*)"transmitCommand(uint8_t command_)");
    transmitBits(command_, COMMAND_SIZE);
}

void transmitAddress(uint8_t address_){
    DEBUG_FUNCTION_CALL((uint8_t*)"transmitAddress(uint8_t address_)");
    transmitBits(address_, ADDRESS_SIZE);
}

// void bitmask(uint8_t position){
//     return _BV(position);
// }


