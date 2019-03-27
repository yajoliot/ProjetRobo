#include "LineTracker.h"

LineTracker::LineTracker(){
    LineTracker::initTracker();
}

void Linetracker::initTracker(){
    DDRA = 0x00;
    DDRC = 0xff;

}

unint8_t LineTracker::readValue(unit8_t pinNumber){
    
    uint8_t result = 0 
	uint8_t readValue = (this->can.lecture (pinNumber) >> 2);

	if (readValue <= COMPARE_VALUE)
        retour = 1;

    return result;
}

void LineTracker::updateValues(){

    for(int i = 0; i < 5; i+)
        this->valueMap |= (LineTracker::readValue(i) << i);

}

uint8_t Linetracker::getValueMap(){
    return valueMap;
}








