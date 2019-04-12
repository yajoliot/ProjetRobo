#ifndef ROBOT_H
#define ROBOT_H

#include "LineTracker.h"
#include "pwm.h"
#include "Minuterie.h"

typedef enum {           /* <--- the type definition */
  INIT = 1, 
  INTWAIT = 2, 
  ANALYSE = 3,
  WAIT_TILL_END = 4,
  END = 5
} etats;

extern volatile etats etat;

class Robot {
    private:
        //Minuterie
        //SIRC sirc;

        

        PWM* pwm;
        LineTracker* lineTracker;
        

    public:

        Robot();
        void* operator new(size_t size);
        void Run(uint8_t IRCom);
        void RunCMD1();
        void RunCMD2();
        void RunCMD3();
        void RunCMD4();
        void RunCMDCoin();
        void test();
};

#endif
