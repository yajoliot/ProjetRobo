#ifndef LIB_DIR_USART_H
#define LIB_DIR_USART_H

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include "memoire_24.h"
#include <string.h>
#include <stdio.h>
class TransmissionUART{
public:
	TransmissionUART(); // Constructeur par defaut, initialise les registres
	static void reinit(); // Permet de reinitialiser les registres au cas ou ils sont changés
	static void transmission(uint8_t donnee);
	static void transmissionUART_entier ( uint8_t donnee ); // TransmissionUART d'un entier
	static void transmissionUART_string(char* donnee); // TransmissionUART d'un string
	static uint8_t transmissionUART_receive( void );
};

#endif 
