#include "LineTracker.h"
#include <util/delay.h>
#include "controlPWM.h"

Class Robot {
    private:

        ControlPWM controlPWM;
        LineTracker lineTracker;

    public:
        Robot();
        void Run();


}