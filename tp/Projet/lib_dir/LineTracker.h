#include <avr/io.h> 
#include "can.h"

const uint16_t COMPARE_VALUE = 512;

Class LineTracker{
    private:
        uint8_t valueMap;
        Can can;
    public:
        LineTracker();
        void initTracker();
        uint8_t readValue();
        void updateValueMap();
        uint8_t getValueMap();
        
}