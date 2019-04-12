#include "minuterie.h"

minuterie::MINUTERIE::MINUTERIE(minuterie_configuration configuration_){
	*configuration = configuration_;
}

minuterie::MINUTERIE::~MINUTERIE(){
	free(configuration);
}

//65535 * 1024 = 8s
//65535 * 256 = 8s/4 = 2s
//65535 * 64 = 2s/4 = 0.5s
//65535 * 8 = 0.5s/8 = 0.065535s
//65535 * 1 = 0.00819s

void minuterie::MINUTERIE::startMinuterie(uint16_t){
    OCR2A = duree;

    //TCCR2A |= (1 << COM2A1);
    //TCCR2A |= (1 << COM2A0);
    TCCR2B |= (1 << WGM20);
    TCCR2B |= (1 << CS20);
    TCCR2B |= (1 << CS22);
    TCCR2C = 0;

}

void minuterie::MINUTERIE::stopMinuterie(){
    TCCR1B &= ~(1 << CS12);
    TCCR1B &= ~(1 << CS11);
    TCCR1B &= ~(1 << CS10);
}

void minuterie::MINUTERIE::resetMinuterie(){
    TCNT1 = 0x0000;
}

