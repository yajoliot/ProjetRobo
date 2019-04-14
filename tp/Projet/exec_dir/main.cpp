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

/////////////////////////////////////////////////////////////////////////////

void startMinuterie(){
    //enable timer1
    TCCR1B |= _BV(CS10);
}

void stopMinuterie(){
    TCCR1B &= ~(_BV(CS10));
}

void resetMinuterie(){
    TCNT1 = 0x0000;
}

////////////////////////////////// RECEIVER //////////////////////////////////

#define _600us 4800
#define _860us 6880
#define _880us 7040
#define _1200us 9600 
#define _1940us 15520
#define _1620us 12960
#define _2400us 19200
#define _3862us 30896

//this function is much less efficient than the #defines...
uint16_t calculateCyclesToWaste_us(uint16_t microseconds){
    return microseconds<<3;
}

uint16_t calculateCyclesToWaste_ms(uint16_t miliseconds){
    // return miliseconds
}

volatile bool high_edge = false;
volatile bool low_edge = false;
volatile uint8_t prev_pin_value;
ISR(PCINT2_vect){
    if(prev_pin_value == (PINC & 0x20)){
        return;
    }
    if(prev_pin_value == 0x20){
        //edge from hi to lo
        low_edge = true;
        prev_pin_value = 0x00;
        //VERIFY HEADER!
        startMinuterie();
        OCR1A = _3862us;
        while( ((TIFR1 & _BV(TOV1))==0x00) && TCNT1 >= _2400us ){
            if(PINC & 0x20){
                PORTD = 0x80;
                _delay_us(1000);
                PORTD = 0x00;
            }
        }

    }else/*prev_pin_value == 0x00*/{
        high_edge = true;
        prev_pin_value = 0x20;
    }
    PORTD = prev_pin_value;  
}


void enablePCINT();
void disablePCINT();
// void receiveHeader();
bool verifyHeader();

/////////////////////////////////////////////////////////////////////////////

int main() {
//Port Setup
    DDRC = 0xDF;
    DDRD = 0xFF;
    DDRB = 0xFF;

//Global interrupt setup

    sei();

//setup

    uint8_t tmp = PINC & 0x20;
    DEBUG_PARAMETER_VALUE((uint8_t*)"PINC", &tmp); //either 0x20 or 0x00;
    prev_pin_value = tmp;
    PORTD = prev_pin_value;
    enablePCINT();
//Test function is below
    for(;;){
        // disablePCINT();
        // uint8_t tmp = PINC & 0x20;
        // DEBUG_PARAMETER_VALUE((uint8_t*)"PINC", &tmp); 
        // enablePCINT();
        // if(verifyHeader()){
            //delay 600 ms
            // startMinuterie();
            // while(TCNT1 <=_880us){
            //     sei();
            //     if(low_edge){
            //         low_edge = false;
            //     }
            // }
        // }
    }
}

////////////////////////////////// RECEIVER //////////////////////////////////

// bool verifyHeader(){
//     ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
//     {
//         if(low_edge){
//             low_edge = false;
//             _delay_us(2400);
//             startMinuterie();
//         }
//     }
//     while(TCNT1 <= _4020us){
//         if(high_edge){
//             ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
//             {
//                 high_edge = false;
//                 while(TCNT1!=_4020us){}
//                 stopMinuterie();
//                 resetMinuterie();
//                 return true;
//             }
//         }
//     }
//     return false;
// }

// void receiveHeader(){
//     pna4602m = PINC & 0x20 ? 0x01 : 0x00;
//     enablePCINT();
//     while(high_edge){_delay_us(0);//DEBUG_INFO((uint8_t*)"nice");}
//     //DEBUG_INFO((uint8_t*)"we out");
//     //we got a low edge
//     startMinuterie();
//     while(TCNT1 <= _2400ms){ _delay_us(0);}
//     while(TCNT1 <= _4020ms){//only gets 2 cycle to get a try
//         //DEBUG_INFO((uint8_t*)"rip");
//         if(high_edge){
//             //DEBUG_INFO((uint8_t*)"im faster");
//             disablePCINT();
//             stopMinuterie(); 
//             resetMinuterie();
//             high_edge = false;
//             break;
//         }
//     }
// }


// void receiveBit(){
//     pna4602m = PINC & 0x20 ? 0x01 : 0x00;
//     enablePCINT();
//     startMinuterie();
//     while(TCNT1 <= _1200us){_delay_us(0);}
//     while(TCNT1 <= _1940us){
//         if(high_edge)
//     }
// }

// void receiveBits(){
//     receiveHeader(); startMinuterie();
//     while(high_edge && TNCT1 < _600us){ _delay_us(0); }
//     //after this there is a low dge
//     for(uint8_t i=0 ; i<COMMAND_SIZE ; i++){
//         receiveBit();
//         while(high_edge && TNCT1 < 880us){ _delay_us(0); }
//     }
// }

void enablePCINT(){
    //Enable that PINB6-7 will trigger an ISR event on any change (edge change i guess)
    PCMSK2 |= _BV(PCINT21);//PCMSK1 |= _BV(PCINT14) | _BV(PCINT15);
    //Enable the whole PINB to trigger the interrupt
    PCICR |= _BV(PCIE2);//_BV(PCIE1);
}

void disablePCINT(){
    PCMSK2 &= ~(_BV(PCINT21));
    PCICR &= ~(_BV(PCIE2));
}

/////////////////////////////////////////////////////////////////////////////

