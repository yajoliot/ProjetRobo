#include "USART.h"

// Routine d'initialisation


TransmissionUART::TransmissionUART() {

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

void TransmissionUART::reinit(){
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

void TransmissionUART::transmissionUART_entier(uint8_t donnee){
	char a[8];
	snprintf(a, sizeof(a), "%d\n", donnee);
	for (uint8_t i = 0; i < sizeof(a); i++){
	   	transmission(a[i]);
    }
}

void TransmissionUART::transmissionUART_string(char  *str){

  while (*str != '\0')
    transmission(*(str++));
}

// procédure qui envoie un octet de la carte vers le PC
void TransmissionUART::transmission( uint8_t donnee ) {
	/* Wait for empty transmit buffer */
   while ( !( UCSR0A & (1<<UDRE1)) );
   /* Put data into buffer, sends the data */
   UDR0 = donnee;
}

uint8_t TransmissionUART::transmissionUART_receive( void ){
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;

}