#include "LineTracker.h"
#include <util/delay.h>
#include "PWM.h"

class Robot {
    private:

        PWM controlPWM;
        LineTracker lineTracker;
        //InfraredCom robot2;
        //uint8_t startingCorner;

    public:
        Robot();
        void Run();


};
