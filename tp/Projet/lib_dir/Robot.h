#include "LineTracker.h"
#include "pwm.h"

class Robot {
    private:
        //Minuterie
        //SIRC sirc;
        PWM* pwm;
        LineTracker* lineTracker;
        

    public:

        Robot();
        void Run(uint8_t IRCom);
        void RunCMD1();
        void RunCMD2();
        void RunCMD3();
        void RunCMD4();
};

