#include "LineTracker.h"

/*void isr_INIT() {
    
    DDRD = 0x00;
    DDRB = 0xff;
    
    
    cli ();
    
    EIMSK |= (1 << INT0);
    
    EICRA |= (1 << ISC01); // EICRA = EICRA | (1 << ISC01)
    
    sei ();
    
}

ISR(INT0_vect){
    
    
    _delay_ms(30);
    etat++;
    EIFR |= (1 << INTF0);
}
*/
LineTracker::LineTracker(){
    this->initTracker();
}

void LineTracker::initTracker(){
    DDRA = 0x00;
    DDRC = 0xff;

}

uint8_t LineTracker::readValue(uint8_t pinNumber){
    
    uint8_t result = 0;
    uint8_t value = (this->canConverter.lecture(pinNumber) >> 2);
    //DEBUG_PARAMETER_VALUE((uint8_t*)"readValue->value", &value);
    if (value <= COMPARE_VALUE){
        result = 1;
    }
    else {
        result = 0;
    }

    return result;
}

void LineTracker::updateValueMap(){

    for(uint8_t i = 0; i < 5; i++){
        uint8_t var = LineTracker::readValue(i);
        uint8_t shifter = 1;
        //DEBUG_PARAMETER_VALUE((uint8_t*)"readValue->value", &var);
        if(var)
            this->valueMap |= (var << i);
        else
            this->valueMap &= ~(shifter << i);
        
        //DEBUG_PARAMETER_VALUE((uint8_t*)"readValue->value", &this->valueMap);
    }
    
}

uint8_t LineTracker::getValueMap(){
    return valueMap;
}








