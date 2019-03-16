#include "debug.h"

void debugFunc(uint8_t *func_name)
{
	char buff[64];
	size_t n =  sprintf(buff,
						"%s %s",
						TAG_FUNCTION_CALL,
						func_name
						);

  if (n > _SIZEOF(buff))
    return;

  transmissionUART_string(buff);
}

void debugParameterValue(uint16_t parameter){
	char buff[64];
	size_t n =  sprintf(buff,
						"%s %d",
						TAG_FUNCTION_CALL,
						parameter
						);
	if (n > _SIZEOF(buff))
		return;
	transmissionUART_string(buff);
}