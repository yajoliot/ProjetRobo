#include "util.h"

/*****************************************************************************
*
*   Function name : variableDelay
*
*   Returns :       void
*
*   Parameters :    uint8_t duration_in_ms -> possible values: {0, 1, 2, ..., 65535} 
*
*   Purpose :       Have a delay function that takes a variable and not a 
*					magic number.
*
*****************************************************************************/
void variableDelay(const uint16_t duration_in_ms){
	for(uint16_t i=0 ; i<duration_in_ms ; i++){
		_delay_ms(1.0);
	}
}

void variableDelay25ms(const uint8_t iterations){
	for(uint8_t i=0 ; i<iterations ; i++){
		_delay_ms(25.0);
	}
}