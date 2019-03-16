#ifndef LIB_DIR_DEBUG_H
#define LIB_DIR_DEBUG_H

#include <avr/io.h>
#include <stdio.h>
#include "util.h"
#include "usart.h"

const uint8_t TAG_FUNCTION_CALL[] = "[FUNCTIION CALL]";
const uint8_t TAG_PARAMETER_VALUE[] = "[PARAMETER VALUE]";

#ifdef DEBUG
	#define DEBUG_FUNCTION_CALL(x) debugFunc(x)
	const TransmissionUART uart; //[UNDEFINED BEHAVIOUR]
#else
	#define DEBUG_FUNCTION_CALL(x) 
	;
#endif

void debugFunc(uint8_t *func_name);


#endif