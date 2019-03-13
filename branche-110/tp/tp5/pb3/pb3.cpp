#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <memoire_24.h>

// Routine d'initialisation
 void initialisationUART ( void ) {

	// 2400 bauds. Nous vous donnons la valeur des deux

	// premier registres pour vous éviter des complications

	UBRR0H = 0;

	UBRR0L = 0xCF;

	// permettre la reception et la transmission par le UART0

	UCSR0A = _BV(RXC0) | _BV(TXC0);

	UCSR0B = _BV(RXEN0) | _BV(TXEN0) ;

	// Format des trames: 8 bits, 1 stop bits, none parity

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00) ;

}

// procédure qui envoie un octet de la carte vers le PC

void transmissionUART ( uint8_t donnee ) {
	/* Wait for empty transmit buffer */
   while ( !( UCSR0A & (1<<UDRE1)) );
   /* Put data into buffer, sends the data */
   UDR0 = donnee;
}


int main() {
	DDRA = 0xff; // PORT A est en mode sortie
	DDRB = 0xff; // PORT B est en mode sortie
	DDRC = 0xff; // PORT C est en mode sortie
	DDRD = 0xff; // PORT D est en mode sortie
  
	initialisationUART ();
	
	Memoire24CXXX memoire = Memoire24CXXX();

	

	uint16_t ;

	for ( i = 0; i < 0xff; i++) {
		
		uint8_t donnee[1];
		memoire.lecture(i, donnee);
		transmissionUART (donnee[0] );

		}
	return 0;
}
