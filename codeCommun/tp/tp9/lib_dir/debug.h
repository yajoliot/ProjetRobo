#ifndef LIB_DIR_DEBUG_H
#define LIB_DIR_DEBUG_H

#include <avr/io.h>
#include <stdio.h>
#include "util.h"
#include "usart.h"

const uint8_t TAG_FUNCTION_CALL[] = "[FUNCTIION CALL]";
const uint8_t TAG_FUNCTION_EXIT[] = "[FUNCTION EXIT]";
const uint8_t TAG_PARAMETER_VALUE[] = "[PARAMETER VALUE]";

#ifdef DEBUG
	#define DEBUG_FUNCTION_CALL(x) debugFunctionCall(x)
	#define DEBUG_FUNCTION_EXIT() debugFunctionExit()
	#define DEBUG_PARAMETER_VALUE(x) debugParameterValue(x)
	#define DEBUG_ERROR() debugError()
	const TransmissionUART uart; //[UNDEFINED BEHAVIOUR]
#else
	#define DEBUG_FUNCTION_CALL(x) 
	#define DEBUG_FUNCTION_EXIT(x) 
	#define DEBUG_PARAMETER_VALUE(x) 
	#define DEBUG_ERROR()
	;
#endif

void debugFunctionCall(uint8_t *func_name);
void debugFunctionExit();
void debugParameterValue(void *parameter);
void debugError();


#endif