#include "minuterie.h"

minuterie::MINUTERIE::MINUTERIE(minuterie_configuration configuration_){
	*configuration = configuration_;
}

minuterie::MINUTERIE::~MINUTERIE(){
	free(configuration);
}

void minuterie::MINUTERIE::startMinuterie(){
    switch(configuration->minuterie_mode_){
        case CTC: TCCR1A |= _BV()
        case PFC: 
    }
}

void minuterie::MINUTERIE::stopMinuterie(){
    TCCR1B &= ~(1 << CS12);
    TCCR1B &= ~(1 << CS11);
    TCCR1B &= ~(1 << CS10);
}

void minuterie::MINUTERIE::resetMinuterie(){
    TCNT1 = 0x0000;
}

