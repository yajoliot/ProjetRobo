#include <avr/io.h> 
#include "can.h"
#include "debug.h"

const uint16_t COMPARE_VALUE = 128;

class LineTracker{
    private:
        uint8_t valueMap;
        can canConverter;
    public:
        LineTracker();
        void initTracker();
        uint8_t readValue(uint8_t pinNumber);
        void updateValueMap();
        uint8_t getValueMap();
        
};
