 
 #include <util/atomic.h>
 #include <util/delay.h>
 #include <stdlib.h>
 #include <avr/interrupt.h>
 #include "pwm.h"
 #include "piezo.h"
 #include "usart.h"
 #include "memoire_24.h"
 #include "usart.h"
 #include "bytecode.h"
 #include "util.h"
 #include "LineTracker.h"
 #include "debug.h"
 #include "Minuterie.h"

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
