/*
TP6 Probleme 1

Par Yan Joliot (1898774) et Xavier Brazeau (1854911) Equipe 8 

Description: Le programme suivant implement un compteur incrementant
10 fois par secondes. Lorsque le compteur arrive a 120 ou quele bouton
est relache la del clignote pendant 0.5 secondes. 2 secondes plus tard
la del rouge s'allume et clignote deux fois secondes 5 fois. Finalement
la lumiere tourne verte une seconde et le robot revient a son etat original.

*/

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 

//Couleurs de la del
const uint8_t ETEIND = 0x00;
const uint8_t ROUGE = 0x01;
const uint8_t VERT = 0x02;

//constante d'initialisation des ports en entree ou sortie
const uint8_t SORTIE = 0xFF;
const uint8_t ENTREE = 0x00;

//initialisation d'une machien a etats
enum etat {INIT, ETAT0, ETAT1, ETAT2, ETAT3};
volatile uint8_t etat = INIT;

int compteur = 0;

//Interupteur Int_0
ISR (INT0_vect) { 	

	_delay_ms (30); 
    etat++;
	EIFR |= (1 << INTF0);
    

}

void initialisation (void) {
	cli (); 

    DDRD = ENTREE;
    DDRB = SORTIE; 
	
	
	EIMSK |= (1 << INT0); 
	
    
	
	EICRA |= (1 << ISC00); 

	sei (); 
}

int main()
{
    
    initialisation();

    for (;;)
    {
        switch (etat)
        {
        case INIT:
            PORTB = ETEIND;
        break;

        case ETAT0:
            PORTB = VERT;
            
            if (compteur < 120)
            {
                _delay_ms(100);
                compteur++;
            }
            else
                etat++;

        break;

        case ETAT1:
            PORTB = VERT;
            _delay_ms(500);
            etat++;
        break;
        
        case ETAT2:

            for(int i=0; i < compteur/4 ; i++)
            {
                PORTB= ROUGE;
                _delay_ms(250);
                PORTB= ETEIND;
                _delay_ms(250);
                PORTB= ROUGE;
                _delay_ms(250);
                PORTB= ETEIND;
            }
            etat++;

        break;

        case ETAT3:
            
            PORTB = VERT;
            _delay_ms(1000);
            etat = INIT;
            
        break;

        default:
            break;
        }
    }

    return 0;
}