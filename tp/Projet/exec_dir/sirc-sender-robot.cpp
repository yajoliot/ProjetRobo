/*
Programme du deuxième robot

Le bouton Interrupt est utilisé pour effectuer l’envoi de signaux à partir de ce robot. Voici le fonctionnement demandé :
Le programme utilise un compteur (un entier) et une minuterie.
Au démarrage du programme, le compteur est à 0 et la minuterie est inactive.
À chaque fois que le bouton Interrupt est appuyé, le compteur est incrémenté de 1 et la minuterie est initialisée à 0 puis démarrée.
Lorsque le compteur est à 9 et qu'il doit être incrémenté, sa valeur devient 1 plutôt que 10.
Si la minuterie atteint 2 secondes, elle est arrêtée et un signal correspondant à la valeur du compteur est émis. La valeur du compteur est ensuite remise à 0.
0
*/

#include <avr/io.h>
#include "Sirc.h"
#include "Minuterie.h"

ISR(TIMER2_OVF_vect){
    overflow_count++;
    if(overflow_count>=OVERFLOW_COUNT_TO_45_MS){
        overflow_count = 0x0000;
        end45msTimer();
        reach_end_45_ms = true;
    }
}

volatile uint8_t compteur = 0;
#define TOP 15625
#define ADDR 1

ISR(INT0_vect){
//    _delay_ms(30);
//    if(PIND & 0x02){
        //increment counter
        compteur++; compteur = (compteur % 9)+1;  
        //initialize minuterie
        TCCR1B |= _BV(CS10) | _BV(CS12);
//    }
}

int main(){
    DDRD = 0xFF;
    EIMSK |= _BV(INT0);
    EICRA |= (1 << ISC01);
    sei();

    for(;;){
        if(TCNT1 == TOP){
            PORTB = 0x01;
            setupSIRC();
            transmit(compteur, ADDR);
            resetRegisters();
            compteur = 0x00;

            TCNT1 = 0x0000;
        }
    }
}