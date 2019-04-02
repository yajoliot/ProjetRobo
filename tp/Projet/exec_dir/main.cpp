#ifndef F_CPU
#define F_CPU 8000000UL
#endif
 
#include <util/atomic.h>
#include <util/delay.h>
#include "del.h"
#include "debug.h"

/*
 ==================================================== 
                    CONFIGURATIONS                    
 ==================================================== 
  1 - Command only (4 bits)                           
  2 - Command and Address (4 bits and 1 bit)          
  3 - Command and Address 12 bit (7 bits and 5 bits)  
 ==================================================== 
*/

//Relics of the past

// #define COMMAND_SIZE 0x07 //We can probably set our own command size to something like 4 bits since 9 commands fits right in 4 bits
// #define ADDRESS_SIZE 0x05 //We can probably just use a set address. Therefore there is no need to even set an address. Maybe we should outright not even use it

//Comment and uncomment as you wish. You should only 
//uncomment one at a time

#define MODE_1
// #define MODE_2
// #define MODE_3

#ifdef MODE_1
#define COMMAND_SIZE 0x04
#define ADDRESS_SIZE 0x00 //this forces the for loop to exit immediately! the compiler should optimize this part of the code and thus the address bits will never be transferred no matter what!
#endif
#ifdef MODE_2
#define COMMAND_SIZE 0x04
#define ADDRESS_SIZE 0x01
#endif
#ifdef MODE_3
#define COMMAND_SIZE 0x07
#define ADDRESS_SIZE 0x05
#endif

//These are useful for the following:

//Variadic function with address size defined to null, if address size is non null then you append those bits in the 45ms frame.
// #define TRANSMIT_COMMAND(...) transmitCommand(VARIADIC_TWO(__VA_ARGS__,)


/*
 ==================================================== 
*/
#define HIGH_MODE 0x01
#define LOW_MODE 0x00

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

void begin45msTimer();
void end45msTimer();

//For one T worth of time -> 600 us / 25 us = 24 cycles
#define CYCLES_PER_T 0x18 //24

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

#define OVERFLOW_COUNT_TO_45_MS 0x3726 //14118 (in integer value)
volatile uint16_t overflow_count = 0x0000;
volatile uint8_t reach_end_45_ms = FALSE;

ISR(TIMER2_OVF_vect){
    overflow_count++;
    if(overflow_count>=OVERFLOW_COUNT_TO_45_MS){
        overflow_count = 0x0000;
        end45msTimer();
        reach_end_45_ms = TRUE;
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

//Test function is below

    //dummy command
    uint8_t command = 0x01;
    //dummy address
    uint8_t address = 0x02; //this will get ignored if in MODE_1

    //Tested function

    transmit(command, address);
    

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
    //Enable counter overflow interrupt 
    TIMSK1 |= _BV(TOIE1); 
}

void disableSIRC(){
    //Sets the only prescaler value we touched to 0
    TCCR1B &= ~(_BV(CS10));
    //Disable counter overflow interrupt
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
    //DEBUG_FUNCTION_CALL((uint8_t*)"transmitHighBit()");
    //Two Cycles @ Hi
    for(uint8_t i=0x00 ; i<0x02; i++){
        transmitOneCycle(HIGH_MODE);
    }
    //One Cycle @ Lo
    transmitOneCycle(LOW_MODE);
}

void transmitLowBit(){
    //DEBUG_FUNCTION_CALL((uint8_t*)"transmitLowBit()");
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
    //DEBUG_FUNCTION_CALL((uint8_t*)"transmitBits(uint8_t command_, uint8_t size_)");
    for(uint8_t i=0x00 ; i<size_ ; i++){
        //if the bit @ the ith position is 1
        uint8_t bitmask = _BV(i);
        //DEBUG_PARAMETER_VALUE((uint8_t*)"_BV(i)", &bitmask);
        if(command_ & bitmask){
            //then transmit a high bit
            transmitHighBit();
        }else{
            transmitLowBit();
        }
    }
}

void transmitCommand(uint8_t command_){
    //DEBUG_FUNCTION_CALL((uint8_t*)"transmitCommand(uint8_t command_)");
    transmitBits(command_, COMMAND_SIZE);
}

void transmitAddress(uint8_t address_){
    //DEBUG_FUNCTION_CALL((uint8_t*)"transmitAddress(uint8_t address_)");
    transmitBits(address_, ADDRESS_SIZE);
}

void transmit(uint8_t command_, uint8_t address_){
    DEBUG_FUNCTION_CALL((uint8_t*)"transmit()")
    for(uint8_t i=0x00 ; i<0x03 ; i++){
        //begin 45 ms timer 3
        begin45msTimer();
        //start sending packet
        transmitHeader();
        transmitCommand(command_);
        transmitAddress(address_);
        //wait till end of 45 ms
        while(!reach_end_45_ms){}
        //reset the boolean for 45 ms
        reach_end_45_ms = FALSE;
        //send again. for loop seems appropriate here
    }
    DEBUG_FUNCTION_EXIT();
}

void begin45msTimer(){
    //Set timer2 to normal mode
    //WGM22:0 = 0 -> you don't need to do anything. all are initialized at 0
    //Enable the timer
    TCCR2B |= _BV(CS20);
    //Enable counter overflow interrupt 
    TIMSK2 |= _BV(TOIE2); 
}

void end45msTimer(){
    TCCR2B &= ~(_BV(CS20));
    TIMSK2 &= ~(_BV(TOIE2));
}
// #define ALLOWED_COMMAND_BITS 0x0F
// #define ALLOWED_ADDRESS_BITS 0x01

// int8_t SIRC::makePacket(uint8_t command_, uint8_t address_) {
//     if( command_ & ~(ALLOWED_COMMAND_BITS ) ) 
//        return -1
//     if( address_ & ~(ALLOWED_ADDRESS_BITS) ) 
//        return -1   
//     return ( (command_ << 1) | address_ ) 
// }

