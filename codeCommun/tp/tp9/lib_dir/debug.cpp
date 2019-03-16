#include "debug.h"

void debugFunctionCall(uint8_t *func_name)
{
	char buff[64];
	size_t n =  sprintf(buff,
						"%s %s",
						TAG_FUNCTION_CALL,
						func_name
						);
	if (n > _SIZEOF(buff))
		return;
	TransmissionUART::transmissionUART_string(buff);
}

void debugFunctionExit()
{
	char buff[64];
	size_t n =  sprintf(buff,
						"%s",
						TAG_FUNCTION_EXIT
						);
	if (n > _SIZEOF(buff))
		return;
	TransmissionUART::transmissionUART_string(buff);
}

void debugParameterValue(void *parameter){
	char buff[64];
	size_t n =  sprintf(buff,
						"%s %d",
						TAG_FUNCTION_CALL,
						*((uint8_t*)parameter)
						);
	if (n > _SIZEOF(buff))
		return;
	TransmissionUART::transmissionUART_string(buff);
}

void debugError(){
	char buff[64];
	size_t n =  sprintf(buff,
						"[ERROR]"
						);
	if (n > _SIZEOF(buff))
		return;
	TransmissionUART::transmissionUART_string(buff);
}