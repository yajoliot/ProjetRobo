
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 


const uint8_t Eteinte = 0x00;
const uint8_t Rouge = 0x01;
const uint8_t Verte = 0x02;
volatile uint8_t etat = 0;
volatile uint8_t compteur = 0;

void partirMinuterie (uint16_t duree);

ISR (INT0_vect) {
	if(etat==0){	
		_delay_ms (30);
		if (PIND & 0x04){
			partirMinuterie(781);
		}
		etat=1;
	}
	EIFR |= (1 << INTF0); 
}


ISR (TIMER1_COMPA_vect) {
	compteur++; // Le compteur incrémente lorsque le bouton poussoir est appuyé
}

void partirMinuterie (uint16_t duree) {
	TCNT1 = 0 ; // Démarrer le compteur à 0
	OCR1A = duree; // Permet de compter à la valeur de durée
	TCCR1A |= _BV(COM1A0); // Mode CTC du timer 1 avec horloge divisée par 1024
	TCCR1B |= _BV(CS12)| _BV(CS10) | _BV(WGM12); // Mode CTC du timer 1 avec horloge divisée par 1024
	TCCR1C = 0; // Mode CTC du timer 1 avec horloge divisée par 1024
	TIMSK1 |= _BV(OCIE1A); // Timer/Counter1: interruption après que la durée est atteinte par le compteur
}

void initialisation (void) {
	cli (); 
	DDRD = 0x00; 
	DDRB= 0xFF;
	EIMSK |= (1 << INT0); 
	EICRA |= (1 << ISC00); 
	sei ();
}

int main() {
	initialisation();
	while (true) {
		switch (etat) {	
			case 0: //DEL eteinte
				PORTB = Eteinte;			
				break;
			case 1:
				do{}
				while(compteur<120 && (PIND & 0x04));
				etat=2;
				break;
			case 2: 
				cli ();
                int nombreDeClignotements=2;// le nombre de clignotement choisi est arbitraire,ici 2 pour 500ms parait suffisant
				for (int i = 0; i < nombreDeClignotements; i++){
					PORTB = Verte;
					_delay_ms(500/nombreDeClignotements);   //Fait clignoter DEL verte "nombreDeClignotements" de fois pour 500ms
					PORTB=Eteinte;
					_delay_ms(500/nombreDeClignotements);		
				}	
				//ne peut pas ajouter des etats, probleme inconnu.
				_delay_ms(2000);//la carte mere ne fait rien pendant 2 secondes.				 
				for (int i = 0; i < (compteur/2); i++){ //boucle qui s'execute compteur/2 fois
					PORTB = Rouge;
					_delay_ms(250); 
                    PORTB = Eteinte;
					_delay_ms(250); 
				}
				PORTB = Verte;
				_delay_ms(1000);//La duree pendant laquelle la DEL est verte,ici 1000ms
				PORTB = Eteinte;
				etat=0;
				compteur=0;
				sei ();
				break;
		}
	}
	return 0;
}