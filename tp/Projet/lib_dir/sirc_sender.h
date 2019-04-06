#ifndef LIB_DIR_SIRC_H
#define LIB_DIR_SIRC_H

#ifndef F_CPU
#define F_CPU 8000000UL
#endif
 
#include <util/atomic.h>
#include <util/delay.h>
#include "del.h"
#include "debug.h"
#include "util.h"

/*
 ==================================================== 
                    CONFIGURATIONS                    
 ==================================================== 
  1 - Command only (4 bits)                           
  2 - Command and Address (4 bits and 1 bit)          
  3 - Command and Address 12 bit (7 bits and 5 bits)  
 ==================================================== 
*/

//Comment and uncomment as you wish. You should only uncomment one at a time

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

/*
 ==================================================== 
*/
#define HIGH_MODE 0x01
#define LOW_MODE 0x00

class SIRC_SENDER{
private:
    void transmitOneCycle(uint8_t mode);
    void transmitHighBit();
    void transmitLowBit();
    void transmitHeader();
    void transmitCommand(uint8_t command_);
    void transmitAddress(uint8_t address_);
    void transmitBits(uint8_t command_, uint8_t size_);
    
public:
    SIRC_SENDER();
    void transmit(uint8_t command_, uint8_t address_);
    static void enableSender();
    static void disableSender();
    static void setupSender();
    static void begin45msTimer();
    static void end45msTimer();
};

#define CYCLES_PER_T 0x18 ////For one T worth of time -> 600 us / 25 us = 24 cycles. 0x18 = 24 cycles.
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
        SIRC_SENDER::disableSender();

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
        SIRC_SENDER::end45msTimer();
        reach_end_45_ms = TRUE;
    }
}

#endif