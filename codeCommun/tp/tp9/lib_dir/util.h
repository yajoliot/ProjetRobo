#ifndef LIB_DIR_UTIL_H
#define LIB_DIR_UTIL_H

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <util/delay.h>

//custom variadic function defines for piezo
//source:https://stackoverflow.com/a/9180314
#define VARIADIC_TWO(a, b, ...) (a), (b)
#define VARIADIC_THREE(a, b, c, ...) (a), (b), (c)

#define _SIZEOF(x) sizeof(x)/sizeof(x[0])
#define INTERRUPTEUR_ON (PIND & 0x04)

void variableDelay(uint16_t duration_in_ms);
void variableDelay25ms(uint8_t iterations);

#endif