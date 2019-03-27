#include <avr/io.h>
#include "couleurDel.h"

int main() {
    DDRD = 0x00;
    DDRB = 0xFF;
    for (;;)
        allumerDEL(AMBRE);
    
    return 0;
}
