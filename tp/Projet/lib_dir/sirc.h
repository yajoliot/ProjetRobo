#ifndef LIB_DIR_SIRC_H
#define LIB_DIR_SIRC_H
#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <avr/io.h>

class SIRC{
private:
	uint8_t command;
	uint8_t address;
	void transmitCommand(uint8_t command_);
	void transmitAddress(uint8_t address_);
	void transmitHeader();
public:
	SIRC();
	void initSIRC();
	void transmit(uint8_t command_, uint8_t address_);
};
#endif