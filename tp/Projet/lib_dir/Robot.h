#include "LineTracker.h"
#include <util/delay.h>
#include "PWM.h"

class Robot {
    private:

        PWM controlPWM;
        LineTracker lineTracker;

    public:
        Robot();
        void Run();


};
