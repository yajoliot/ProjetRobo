#include "debug.h"

void debugFunctionCall(uint8_t *function_name)
{
	char buff[64];
	size_t n =  sprintf(buff,
						"%s %s\n",
						TAG_FUNCTION_CALL,
						function_name
						);
	if (n > _SIZEOF(buff))
		return;
	TransmissionUART::transmissionUART_string(buff);
}

void debugFunctionExit()
{
	char buff[64];
	size_t n =  sprintf(buff,
						"%s\n",
						TAG_FUNCTION_EXIT
						);
	if (n > _SIZEOF(buff))
		return;
	TransmissionUART::transmissionUART_string(buff);
}

void debugParameterValue(uint8_t *parameter_name, void *parameter){
	char buff[64];
	size_t n =  sprintf(buff,
						"\t%s %s : 0x%02X\n",
						TAG_PARAMETER_VALUE,
						parameter_name,
						*((uint8_t*)parameter)
						);
	if (n > _SIZEOF(buff))
		return;
	TransmissionUART::transmissionUART_string(buff);
}

void debugError(){
	char buff[64];
	size_t n =  sprintf(buff,
						"%s\n",
						TAG_ERROR
						);
	if (n > _SIZEOF(buff))
		return;
	TransmissionUART::transmissionUART_string(buff);

	for(;;){
		allumerDEL(ROUGE);
		_delay_ms(1000); //there's no need to create a variable for 1 second right now
		allumerDEL(ETEIND);
		_delay_ms(1000);
	}
}

void debugInfo(uint8_t *message){
	char buff[64];
	size_t n =  sprintf(buff,
						"%s %s\n",
						TAG_INFO,
						message
						);
	if (n > _SIZEOF(buff))
		return;
	TransmissionUART::transmissionUART_string(buff);
}