#ifndef ROBOT_H
#define ROBOT_H

#include "LineTracker.h"
#include "pwm.h"
#include "Minuterie.h"
#include "piezo.h"

typedef enum {           /* <--- the type definition */
  INIT = 0, 
  INTWAIT = 1, 
  ANALYSE = 2,
  WAIT_TILL_END = 3,
  END = 4,
  NEXT = 5
} etats;

extern volatile bool boolISR;
extern volatile etats etat;
extern volatile uint8_t pointCounterISR;
extern volatile uint8_t cornerCounterISR;

class Robot {
    private:
        //SIRC sirc;

        
        uint8_t IRCom = 0;
        uint8_t nCMD = 0;
        PWM* pwm;
        LineTracker* lineTracker;
        

    public:

        Robot();
        ~Robot();
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
};

#endif
