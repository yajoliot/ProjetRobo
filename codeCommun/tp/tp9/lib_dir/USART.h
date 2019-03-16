#ifndef LIB_DIR_USART_H
#define LIB_DIR_USART_H

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h> 
#include <util/delay.h>

void initialisationUART (void);
void transmissionUART (uint8_t donnee);

#endif 
