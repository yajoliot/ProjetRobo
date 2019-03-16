#include "debug.h"

void debugFunc(uint8_t *func_name)
{
  char buff[64];

  size_t n =  sprintf(buff,
		      "%s %s",
		      TAG_FUNC_CALL,
		      func_name
		      );

  if (n > 64)
    return;

  //uart.transmissionUART_string(buff);
}