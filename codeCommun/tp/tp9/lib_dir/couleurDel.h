
#ifndef LIB_DIR_COULEURDEL_H
#define LIB_DIR_COULEURDEL_H

#include <avr/io.h> 
#include <util/delay.h>

const uint8_t ETEIND = 0x00;
const uint8_t ROUGE = 0x01;
const uint8_t VERT = 0x02;
const uint8_t AMBRE = 0x03;

void allumerDEL(uint8_t Couleur);


#endif
