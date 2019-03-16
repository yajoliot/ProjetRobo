#ifndef LIB_DIR_UTIL_H
#define LIB_DIR_UTIL_H

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <util/delay.h>

#define _SIZEOF(x) sizeof(x)/sizeof(x[0])

void variableDelay(uint16_t duration_in_ms);

#endif