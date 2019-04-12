#include "minuterie.h"

MINUTERIE::MINUTERIE(minuterie_configuration configuration_){
	*configuration = configuration_;
}

MINUTERIE::~MINUTERIE(){
	free(configuration);
}

//65535 * 1024 = 8s
//65535 * 256 = 8s/4 = 2s
//65535 * 64 = 2s/4 = 0.5s
//65535 * 8 = 0.5s/8 = 0.065535s
//65535 * 1 = 0.00819s

void MINUTERIE::startMinuterie(uint16_t duree){
    OCR2A = duree;

    //TCCR2A |= (1 << COM2A1);
    //TCCR2A |= (1 << COM2A0);
    TCCR2B |= (1 << WGM20);
    TCCR2B |= (1 << CS20);
    TCCR2B |= (1 << CS22);
    TCCR2C = 0;

}

void MINUTERIE::stopMinuterie(){
    TCCR2B &= ~(1 << CS22);
    TCCR2B &= ~(1 << CS21);
    TCCR2B &= ~(1 << CS20);
}

void MINUTERIE::resetMinuterie(){
    TCNT2 = 0x0000;
}

