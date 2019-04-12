#ifndef LIB_DIR_ISR_H
#define LIB_DIR_ISR_H

#include "sirc_sender.h"

#define OVERFLOW_COUNT_TO_45_MS 0x3726 //14118 (in integer value)

ISR(TIMER2_OVF_vect){
    // DEBUG_INFO((uint8_t*)"this shud work");
    uint8_t tmp = overflow_count;
    // DEBUG_PARAMETER_VALUE((uint8_t*)"ovfcount define", &tmp);
    overflow_count++;
    if(overflow_count>=OVERFLOW_COUNT_TO_45_MS){
        DEBUG_INFO((uint8_t*)"we made it in");
        overflow_count = 0x0000;
        SIRC_SENDER::end45msTimer();
        SIRC_SENDER::reach_end_45_ms = true;
    }
}

// ISR(TIMER1_OVF_vect){
// 	count_++;
// }

#endif