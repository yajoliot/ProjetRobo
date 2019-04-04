#include <Minuterie.h>




void startMinuterie (uint16_t duree){
    

    // mode CTC du timer 1 avec horloge divisée par 1024

    // interruption après la durée spécifiée

    TCNT1 = 0 ;

    // OCR1A = duree;

    // TCCR1A |= (1 << COM1A1);
    // TCCR1A |= (1 << COM1A0);
    // TCCR1B |= (1 << WGM10);
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    TCCR1C = 0;

    // TIMSK1 |= (1 << OCIE1A);


}

void resetMinuterie ()
{
    
    TIMSK1 = (0 << OCIE1A);
}
