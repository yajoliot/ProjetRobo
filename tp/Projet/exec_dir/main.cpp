#include <avr/io.h>


int main() {
    
    DDRC = 0xff;

    PINC0 = 1;
    PINC1 = 1;
    PINC2 = 1;
    PINC3 = 1;
    PINC4 = 1;
}
