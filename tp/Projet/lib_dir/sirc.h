#ifndef SIRC_H
#define SIRC_H

#include "Minuterie.h"
#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>


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


#define _500us 4000
#define _600us 4800
#define _860us 6880
#define _880us 7040
#define _1200us 9600 
#define _1986us 15888
#define _1620us 12960
#define _2400us 19200
#define _3862us 31000


#define threshold 11840//18504

extern volatile bool highEdge;
extern volatile bool lowEdge;
extern volatile bool headerDetected;
extern volatile uint8_t prev_pin_value;

#define OVERFLOW_COUNT_TO_45_MS 0x3726 //14118 (in integer value)

extern volatile uint16_t overflow_count;
extern volatile bool reach_end_45_ms;

uint8_t readBit();
uint8_t readBits(uint8_t length);
bool headerVerified();
void enablePCINT();
void disablePCINT();

void transmitOneCycle(uint8_t mode);
void transmitHighBit();
void transmitLowBit();
void transmitHeader();
void transmitCommand(uint8_t command_);
void transmitAddress(uint8_t address_);
void transmitBit(uint8_t bit);
void transmitEndBit();
void transmit(uint8_t command_, uint8_t address_);
void enableSIRC();
void disableSIRC();
void setupSIRC();
void begin45msTimer();
void end45msTimer();

#endif

