#include "Sirc.h"

//(GREEN -> NOTHING -> GREEN):
// 0 bit:      - 35k-37.5k
// 1 bit:      - 
//(GREEN -> RED -> GREEN):
// 0 bit:      - 
// 1 bit:      -  


// uint16_t calculateCyclesToWaste_ms(uint16_t miliseconds){
    // return miliseconds
// }


volatile bool highEdge = false;
volatile bool lowEdge = false;
volatile bool headerDetected = false;
volatile uint8_t prev_pin_value = (PINC & 0x20);


bool headerVerified(){

        stopMinuterieIR(); resetMinuterieIR(); startMinuterieIR();
        while(TCNT1 < _1200us){
            prev_pin_value = (PINC & 0x20);
            if(prev_pin_value == 0x20){
                return false;
            }
        }

        stopMinuterieIR(); resetMinuterieIR();
        while((PINC & 0x20)== 0x00){}

        startMinuterieIR();
        while(TCNT1 < 4000){
            prev_pin_value = (PINC & 0x20);
            if(prev_pin_value == 0x00){
                return false;
            }
        }

        stopMinuterieIR(); resetMinuterieIR();
        while(PINC & 0x20){}
        prev_pin_value = 0x00;
        PORTB = 0x02;
        return true;

}

uint8_t readBit(){
    stopMinuterieIR(); resetMinuterieIR(); startMinuterieIR();
    while((PINC & 0x20) == 0x00){}
    stopMinuterieIR();
    while(PINC & 0x20){}
    if(TCNT1 < threshold){
        return 0; // 0
    }else// if( TCNT1 >= threshold){
        return 1; // 1
    //}

}

uint8_t readBits(uint8_t length){
    uint8_t result = 0x00;
    for(uint8_t i=0 ; i<length ; i++){
        
        result |= (readBit() << i);

    }
    return result;
}



////////////////////////////////// RECEIVER //////////////////////////////////

void enablePCINT(){
    //Enable that PINB6-7 will trigger an ISR event on any change (edge change i guess)
    PCMSK2 |= _BV(PCINT21);//PCMSK1 |= _BV(PCINT14) | _BV(PCINT15);
    //Enable the whole PINB to trigger the interrupt
    PCICR |= _BV(PCIE2);//_BV(PCIE1);
}

void disablePCINT(){
    PCMSK2 &= ~(_BV(PCINT21));
    PCICR &= ~(_BV(PCIE2));
}

/////////////////////////////////////////////////////////////////////////////

volatile uint16_t overflow_count = 0x0000;
volatile bool reach_end_45_ms = false;

void openPin(){
    TCCR1A |= _BV(COM1A0);
}

void closePin(){
    TCCR1A &= ~(_BV(COM1A0));
}

void transmitBit(uint8_t bit){
    openPin();
    if(bit == 1){
        enableSIRC();
        _delay_us(1200);
        disableSIRC();
    }else/*bit == 0x01*/{
        enableSIRC();
        _delay_us(600);
        disableSIRC();
    }
    closePin();
    PORTD &= ~(_BV(PD5));
    //maybe need to set OC1A to 0?
    _delay_us(600);
}

void setupSIRC(){
    //Set the registers : CTC mode, Toggle on Compare, No prescaler
    // TCCR1A |= _BV(COM1A0);
    TCCR1B |=  _BV(WGM12);
    //TCCR1B |= _BV(CS10);
    
    //Set the frequency : 38 kHz
    //The following value is what we will use to set the "one"
    OCR1A = 0x68;
}

void enableSIRC(){
    TCNT1 = 0x0000;
    //Non 0 value for the prescaler enables the timer
    TCCR1B |= _BV(CS10);
    //Enable counter overflow interrupt 
    // TIMSK1 |= _BV(TOIE1); 
}

void disableSIRC(){
    //Sets the only prescaler value we touched to 0
    TCCR1B &= ~(_BV(CS10));
    //Disable counter overflow interrupt
    TIMSK1 &= ~(_BV(TOIE1));
    // cli();
}

void transmitBits(uint8_t command_, uint8_t size_){
    //DEBUG_FUNCTION_CALL((uint8_t*)"transmitBits(uint8_t command_, uint8_t size_)");
    for(uint8_t i=0x00 ; i<size_ ; i++){
        //if the bit @ the ith position is 1
        // uint8_t bitmask = _BV(i);
        // //DEBUG_PARAMETER_VALUE((uint8_t*)"_BV(i)", &bitmask);
        if(command_ & _BV(i)){
            //then transmit a high bit
            transmitBit(1);
        }else{
            transmitBit(0);
        }
    }
}

void transmitHeader(){
    openPin(); 
    enableSIRC();
    _delay_us(2400);
    disableSIRC();
    closePin();
    PORTD &= ~(_BV(PD5));
    _delay_us(600);
}

void transmitCommand(uint8_t command_){
    transmitBits(command_, COMMAND_SIZE);
}

void transmitAddress(uint8_t address_){
    transmitBits(address_, ADDRESS_SIZE);
}

void transmitEndBit(){
    transmitBit(1);
}

void transmit(uint8_t command_, uint8_t address_){
    // DEBUG_FUNCTION_CALL((uint8_t*)"transmit()");
    for(uint8_t i=0x00 ; i<0x03 ; i++){
        //begin 45 ms timer 3
        begin45msTimer();
        //start sending packet
        transmitHeader();
        transmitCommand(command_);
        transmitAddress(address_);
        // transmitEndBit();
        //wait till end of 45 ms
        while(!reach_end_45_ms){}
        //reset the boolean for 45 ms
        reach_end_45_ms = false;
        //send again. for loop seems appropriate here
    }
    // DEBUG_FUNCTION_EXIT();
}

void begin45msTimer(){
    //Set timer2 to normal mode
    //WGM22:0 = 0 -> you don't need to do anything. all are initialized at 0
    //Enable the timer
    TCCR2B |= _BV(CS20);
    //Enable counter overflow interrupt 
    TIMSK2 |= _BV(TOIE2); 
}

void end45msTimer(){
    TCCR2B &= ~(_BV(CS20));
    TIMSK2 &= ~(_BV(TOIE2));
}

