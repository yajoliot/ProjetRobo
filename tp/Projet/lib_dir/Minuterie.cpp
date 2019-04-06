#include <Minuterie.h>




void startMinuterie (uint16_t duree){
    

    // mode CTC du timer 1 avec horloge divisée par 1024

    // interruption après la durée spécifiée

    TCNT1 = 0 ;

    OCR1A = duree;

    TCCR1A |= (1 << COM1A1);
    TCCR1A |= (1 << COM1A0);
    TCCR1B |= (1 << WGM10);
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    TCCR1C = 0;

    TIMSK1 |= (1 << OCIE1A);


}

void stopMinuterie(){
    TCCR1B &= (0 << CS12);
    TCCR1B &= (0 << CS11);
    TCCR1B &= (0 << CS10);
}

void resetMinuterie ()
{
    TCNT &= 0x00;
}
