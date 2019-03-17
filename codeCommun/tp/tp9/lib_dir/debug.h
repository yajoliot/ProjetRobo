#ifndef LIB_DIR_DEBUG_H
#define LIB_DIR_DEBUG_H

#include <avr/io.h>
#include <stdio.h>
#include "util.h"
#include "usart.h"
#include "del.h"

#include <avr/pgmspace.h>

const uint8_t TAG_FUNCTION_CALL[] = "[FUNCTION CALL]";
const uint8_t TAG_FUNCTION_EXIT[] = "[FUNCTION EXIT]";
const uint8_t TAG_PARAMETER_VALUE[] = "[PARAMETER VALUE]";
const uint8_t TAG_INFO[] PROGMEM = "[INFO]";

#ifdef DEBUG
	#define DEBUG_FUNCTION_CALL(x) debugFunctionCall(x)
	#define DEBUG_FUNCTION_EXIT() debugFunctionExit()
	#define DEBUG_PARAMETER_VALUE(x,y) debugParameterValue(x,y)
	#define DEBUG_ERROR() debugError()
	#define DEBUG_INFO(x) debugInfo(x)
	const TransmissionUART uart; //[UNDEFINED BEHAVIOUR]
#else
	#define DEBUG_FUNCTION_CALL(x) 
	#define DEBUG_FUNCTION_EXIT(x) 
	#define DEBUG_PARAMETER_VALUE(x,y) 
	#define DEBUG_ERROR()
	#define DEBUG_INFO(x)
	;
#endif

void debugFunctionCall(uint8_t *function_name);
void debugFunctionExit();
void debugParameterValue(uint8_t *parameter_name, void *parameter);
void debugError();
void debugInfo();

void rando();

#endif