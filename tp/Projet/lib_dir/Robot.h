#ifndef ROBOT_H
#define ROBOT_H

#include "LineTracker.h"
#include "pwm.h"
#include "Minuterie.h"
#include "piezo.h"
#include "Sirc.h"



extern volatile bool boolISR;
extern volatile uint8_t pointCounterISR;
extern volatile uint8_t cornerCounterISR;
extern volatile bool useCornerISR;
extern volatile bool usePointISR;
extern volatile bool intWaitISR;

class Robot {
    private:
        //SIRC sirc;

        
        uint8_t IRCom = 0;
        uint8_t nCMD = 0;
        PWM pwm;
        LineTracker lineTracker;
        

    public:

        Robot();
        void operator delete(void* obj);
        void* operator new(size_t size);
        void Run(uint8_t IRCom);
        void RunCMD1();
        void RunCMD2();
        void RunCMD3();
        void RunCMD4();
        void RunCMDCoin();
        void test();
        uint8_t receive();
        void isr_INIT();
};

#endif
