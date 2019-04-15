#ifndef LIB_DIR_BYTECODE_H
#define LIB_DIR_BYTECODE_H

#include "memoire_24.h"
#include "USART.h"
#include "constantes.h"

void lireDonnees(uint16_t& address, Memoire24CXXX &_memoire, uint8_t & instruction, uint8_t & operande);
void ecrireDonnees(Memoire24CXXX &_memoire, TransmissionUART &_uart);

#endif