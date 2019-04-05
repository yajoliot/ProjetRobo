static void SIRC::setupSIRC(){

    //Set the registers : CTC mode, Toggle on Compare, No prescaler
    TCCR1A |= _BV(WGM12) | _BV(COM1A0);
    //TCCR1B |= _BV(CS10);
    
    //Set the frequency : 40 kHz
    //The following value is what we will use to set the "one"
    OCR1A = 0x63;

}

static void SIRC::enableSIRC(){
    //Non 0 value for the prescaler enables the timer
    TCCR1B |= _BV(CS10);
    //Enable counter overflow interrupt 
    TIMSK1 |= _BV(TOIE1); 
}

static void SIRC::disableSIRC(){
    //Sets the only prescaler value we touched to 0
    TCCR1B &= ~(_BV(CS10));
    //Disable counter overflow interrupt
    TIMSK1 &= ~(_BV(TOIE1));
    // cli();
}

void SIRC::ransmitOneCycle(uint8_t mode){
    // DEBUG_FUNCTION_CALL((uint8_t*)"transmitOneCycle()");
    //Set the PWM
    enableSIRC();
    //Wait until ISR ends itself the 24 cycles for 1 T
    while(!reach_end_T){}
    reach_end_T = FALSE;

    // uint8_t tmp = TCCR1B;
    // DEBUG_PARAMETER_VALUE((uint8_t*)"TCCR1B",&tmp);
    // DEBUG_FUNCTION_EXIT();
}

void SIRC::transmitHighBit(){
    //DEBUG_FUNCTION_CALL((uint8_t*)"transmitHighBit()");
    //Two Cycles @ Hi
    for(uint8_t i=0x00 ; i<0x02; i++){
        transmitOneCycle(HIGH_MODE);
    }
    //One Cycle @ Lo
    transmitOneCycle(LOW_MODE);
}

void SIRC::transmitLowBit(){
    //DEBUG_FUNCTION_CALL((uint8_t*)"transmitLowBit()");
    //One Cycle @ Hi
    transmitOneCycle(HIGH_MODE);
    //One Cycle @ Lo
    transmitOneCycle(LOW_MODE);
}

void SIRC::transmitHeader(){
    //Four Cycles @ Hi
    for(uint8_t i=0x00 ; i<0x04 ; i++){
        transmitOneCycle(HIGH_MODE);
    }
    //One Cycle @ Lo
    transmitOneCycle(LOW_MODE);
}


void SIRC::transmitBits(uint8_t command_, uint8_t size_){
    //DEBUG_FUNCTION_CALL((uint8_t*)"transmitBits(uint8_t command_, uint8_t size_)");
    for(uint8_t i=0x00 ; i<size_ ; i++){
        //if the bit @ the ith position is 1
        uint8_t bitmask = _BV(i);
        //DEBUG_PARAMETER_VALUE((uint8_t*)"_BV(i)", &bitmask);
        if(command_ & bitmask){
            //then transmit a high bit
            transmitHighBit();
        }else{
            transmitLowBit();
        }
    }
}

void SIRC::transmitCommand(uint8_t command_){
    //DEBUG_FUNCTION_CALL((uint8_t*)"transmitCommand(uint8_t command_)");
    transmitBits(command_, COMMAND_SIZE);
}

void SIRC::transmitAddress(uint8_t address_){
    //DEBUG_FUNCTION_CALL((uint8_t*)"transmitAddress(uint8_t address_)");
    transmitBits(address_, ADDRESS_SIZE);
}

void SIRC::transmit(uint8_t command_, uint8_t address_){
    DEBUG_FUNCTION_CALL((uint8_t*)"transmit()");
    for(uint8_t i=0x00 ; i<0x03 ; i++){
        //begin 45 ms timer 3
        begin45msTimer();
        //start sending packet
        transmitHeader();
        transmitCommand(command_);
        transmitAddress(address_);
        //wait till end of 45 ms
        while(!reach_end_45_ms){}
        //reset the boolean for 45 ms
        reach_end_45_ms = FALSE;
        //send again. for loop seems appropriate here
    }
    DEBUG_FUNCTION_EXIT();
}

void SIRC::begin45msTimer(){
    //Set timer2 to normal mode
    //WGM22:0 = 0 -> you don't need to do anything. all are initialized at 0
    //Enable the timer
    TCCR2B |= _BV(CS20);
    //Enable counter overflow interrupt 
    TIMSK2 |= _BV(TOIE2); 
}

void SIRC::end45msTimer(){
    TCCR2B &= ~(_BV(CS20));
    TIMSK2 &= ~(_BV(TOIE2));
}

