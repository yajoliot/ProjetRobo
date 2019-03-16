#ifndef LIB_DIR_DEBUG_H
#define LIB_DIR_DEBUG_H

#include <avr/io.h>
#include <stdio.h>

const uint8_t TAG_FUNC_CALL[] = "[FUNC_CALL]"

#ifdef DEBUG
	#define DEBUG_FUNC(x) debugFunc(x)
	;
#else
	#define DEBUG_FUNC(x) 
	;
#endif

void debugFunc(uint8_t *func_name);


#endif