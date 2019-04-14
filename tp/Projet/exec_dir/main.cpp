#ifndef F_CPU
#define F_CPU 8000000UL
#endif
 
#include <util/atomic.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "del.h"
#include "debug.h"

/////////////////////////////////////////////////////////////////////////////

#define MODE_1
// #define MODE_2
// #define MODE_3

#ifdef MODE_1
#define COMMAND_SIZE 0x04
#define ADDRESS_SIZE 0x00
#endif
#ifdef MODE_2
#define COMMAND_SIZE 0x04
#define ADDRESS_SIZE 0x01
#endif
#ifdef MODE_3
#define COMMAND_SIZE 0x07
#define ADDRESS_SIZE 0x05
#endif

/////////////////////////////////////////////////////////////////////////////

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

void setPrescaler(uint8_t pos){
    TCCR1B |= _BV(pos);
}

////////////////////////////////// RECEIVER //////////////////////////////////

#define _500us 4000
#define _600us 4800
#define _860us 6880
#define _880us 7040
#define _1200us 9600 
#define _1986us 15888
#define _1620us 12960
#define _2400us 19200
#define _3862us 31000

#define lo_borne 14080 // 14.4k
#define hi_borne 22928 //23k
#define threshold 0//18504


//this function is much less efficient than the #defines...
uint16_t calculateCyclesToWaste_us(uint16_t microseconds){
    return microseconds<<3;
}

// uint16_t calculateCyclesToWaste_ms(uint16_t miliseconds){
    // return miliseconds
// }

uint8_t readBits(uint8_t length);
bool verifyHeader();
bool isInfraredSending();
volatile bool highEdge = false;
volatile bool lowEdge = false;
volatile bool headerDetected = false;
volatile uint8_t prev_pin_value;
ISR(PCINT2_vect){
    if(prev_pin_value == 0x20){
        //edge from hi to lo
        prev_pin_value = 0x00;
        // //VERIFY HEADER!
        // headerDetected = verifyHeader();
        // if(headerDetected){
        //     for(;;){
        //         PORTB = 0x01;
        //         _delay_ms(3000);
        //         PORTB = 0x00;
        //         _delay_ms(3000);
        //     }   
        // }
    }else/*prev_pin_value == 0x00*/{
        // highEdge = true;
        prev_pin_value = 0x20;
    }
    // PORTD = prev_pin_value; 
}

void testFunction(){
    PORTD = 0x00;
    stopMinuterie(); resetMinuterie(); startMinuterie();
    while(TCNT1 < 4000){
    }
    stopMinuterie();resetMinuterie(); 
    PORTD = 0xff;
    startMinuterie();
    while(TCNT1 < 4000){
    }
    stopMinuterie();resetMinuterie();
}

bool verifyHeader(){

        stopMinuterie(); resetMinuterie(); startMinuterie();
        while(TCNT1 < _1200us){
            prev_pin_value = (PINC & 0x20);
            if(prev_pin_value == 0x20){
                return false;
            }
        }

        stopMinuterie(); resetMinuterie();
        while((PINC & 0x20)== 0x00){}

        startMinuterie();
        while(TCNT1 < 4000){
            prev_pin_value = (PINC & 0x20);
            if(prev_pin_value == 0x00){
                return false;
            }
        }

        stopMinuterie(); resetMinuterie();
        while(PINC & 0x20){}
        prev_pin_value = 0x00;
    // PORTB = 0x01;
        return true;


        // while((prev_pin_value == 0x00) && (TCNT1 <= _3862us)){//TCNT1 <= _3862us
        //     //polling
        //     prev_pin_value = (PINC & 0x20);
        //     if(prev_pin_value == 0x20){
        //         stopMinuterie(); resetMinuterie(); startMinuterie();
        //         while(TCNT1 < _600us && (prev_pin_value == 0x20)){ prev_pin_value = (PINC & 0x20); }
        //         while((prev_pin_value == 0x20) && (TCNT1 <= _880us)){
        //             prev_pin_value = (PINC & 0x20);
        //             if(prev_pin_value == 0x00){
        //                 stopMinuterie(); resetMinuterie();   
        //                 return true;
        //             }
        //         }
        //     }
        // }
        // return false;
}

uint8_t readBit(){
    stopMinuterie(); resetMinuterie(); startMinuterie();
    while((PINC & 0x20) == 0x00){}
    while(PINC & 0x20){}
    stopMinuterie();
    if(TCNT1 < threshold){
        return 0; // 0
    }else if( TCNT1 >= threshold){
        return 1; // 1
    }
}

uint8_t readBits(uint8_t length){
    uint8_t result = 0x00;
    for(uint8_t i=0 ; i<length ; i++){
        result = result << 1;
        uint8_t bit = readBit();
        if(bit==0){}
        else//bit==0x01
            result |= readBit();
    }
    return result;
}

void enablePCINT();
void disablePCINT();
// void receiveHeader();

/////////////////////////////////////////////////////////////////////////////

int main() {
//Port Setup
    DDRA = 0xFF;
    DDRC = 0xDF;
    DDRD = 0xFF;
    DDRB = 0xFF;

//setup

    uint8_t tmp = PINC & 0x20;
    DEBUG_PARAMETER_VALUE((uint8_t*)"PINC", &tmp); //either 0x20 or 0x00;
    prev_pin_value = tmp;
    // PORTD = prev_pin_value;
    sei();
    enablePCINT();
//Test function is below
    for(;;){
        // testFunction();
        // if(headerDetected){
            // headerDetected = false;
        // }
        // if(headerDetected)
        // {
            // disablePCINT();
            // abort();
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

