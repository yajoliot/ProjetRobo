
#define F_CPU 8000000	
#include <avr/io.h>
#include <util/delay.h>

using namespace std;





void initialisationUART (void) {

	// 2400 bauds. Nous vous donnons la valeur des deux premier registres pour vous éviter des complications
	UBRR0H = 0;
	UBRR0L = 0xCF;
	
	// permettre la reception et la transmission par le UART0
	UCSR0A |= (1 << RXC0) | (1 << TXC0);
	UCSR0B |= (1 << RXEN0)|(1 << TXEN0); 
	
	// Format des trames: 8 bits, 1 stop bits, none parity
	UCSR0C |= (1<< UCSZ01)|(1 << UCSZ00); 
}

// De l'USART vers le PC

void transmissionUART (uint8_t donnee) {
	while (!( UCSR0A & (1 << UDRE0)))  //p.174
	{};
	UDR0 = donnee;
}


int main() {
	
	initialisationUART();
	char mots[21] = "Le robot en INF1995\n";
	uint8_t i, j;
    _delay_ms(10000*5);
	for ( i = 0; i < 100; i++ ) {
		for ( j=0; j < 20; j++ ){
            char mots[21] = "Le robot en INF1995\n";
	
			transmissionUART (mots[j]);
        }
	}
	return 0;
}