#include "Minuterie.h"




void startMinuterie (uint16_t duree){
    

    // mode CTC du timer 1 avec horloge divisée par 1024

    // interruption après la durée spécifiée


    OCR1A = duree;

    TCCR1A |= (1 << COM1A1);
    TCCR1A |= (1 << COM1A0);
    TCCR1B |= (1 << WGM10);
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    TCCR1C = 0;

    


}

void resetRegisters(){
    TCCR1A &= ~(1 << COM1A1);
    TCCR1A &= ~(1 << COM1A0);
    TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B &= ~(1 << WGM12);
    TCCR1B &= ~(1 << WGM13);
    TCCR1B &= ~(1 << CS10);
    TCCR1B &= ~(1 << CS12);

}

void stopMinuterie(){
    TCCR1B &= (0 << CS12);
    TCCR1B &= (0 << CS11);
    TCCR1B &= (0 << CS10);
}

void resetMinuterie ()
{
    TCNT1 &= 0x00;
}

void startMinuterieIR(){
    //enable timer1
    TCCR1B |= _BV(CS10);
}

void stopMinuterieIR(){
    TCCR1B &= ~(_BV(CS10));
}

void resetMinuterieIR(){
    TCNT1 = 0x0000;
}

void setPrescalerIR(uint8_t pos){
    TCCR1B |= _BV(pos);
}