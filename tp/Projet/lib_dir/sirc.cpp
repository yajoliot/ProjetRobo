#include "sirc.h"

//we want our SIRC to be quicker.
//it will thus run the 12 bit version of the protocol

SIRC::SIRC(){
	initSIRC();
}

void SIRC::initSIRC(){
	//insert CTC mode PWM here
}

void SIRC::transmitCommand(uint8_t command_){
	//i'm guessing i'll moduralize
	//7 bits 
}

void SIRC::transmitAddress(uint8_t address_){

}

void SIRC::transmitHeader(){
	//4T high, 1T low
	//2400 microseconds at high, 600 microseconds at low @ 40kHz
}

void SIRC::transmit(uint8_t command_, uint8_t address_){

}