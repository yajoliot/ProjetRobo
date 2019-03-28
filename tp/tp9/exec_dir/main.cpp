#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <util/atomic.h>
#include "pwm.h"
#include "piezo.h"
#include "usart.h"
#include "memoire_24.h"
#include "usart.h"
#include "bytecode.h"

//Setup is as such

// Pwm/Piezo are the same as in the modules branch:

// MOTOR PWM
// PD2 + PD4 (identified as 1 and 5 on the board) for roueGauche
// PD5 + PD7 (identified as 6 and 8 on the board) for roueDroite
// PD2: direction 
// PD4: pwm
// PD5: pwm
// PD7: direction
// Plug these so that the direction pin is connected to the D pin on the pont H for either roue and the pwm pin to the E pin on either roue 

// PIEZO PWM
// PD3 or PD4 create the PWM (or... Pin(5) and Pin(6) as identified on the board)
// Choose any of the surrounding pins to set to ground and pass it to the constructor along with the PD3 or PD4
// Plug the piezo in any way but on those pins you've passed to the constructor

// DEL LIGHT
// (+) sign -> PINB0 (identified as 1 on the board)
// (-) sign -> PINB1 (identified as 2 on the board)

// CAVALIERS
// - IntEN has to have a cavalier

// POWER SUPPLY
// - Motherboard: Can be supplied by USB. We are not dealing with CAN. So it's not that serious.
// - Pont H: Needs preferably new AA batteries (6). If not try to use the power supply in the labs.

// NOTES:
// 1. Motor needs the knob to be pulled backwards (towards the Pont H) to be supplied by the batteries
// 2. Clicking on the interrupt will switch the whole thing into the WRITE_MODE which is basically the mode to write the bytecode into the EEPROM
//     2.1. You must reset/restart to get back to the READ_MODE
// 3. Les fonctions DEBUG_... sont les fonctions de debogage. Elles permettent d'envoyer des messages de diagnostique a l'ordinateur branchee qui a serieViaUSB d'ouvert.
//     3.1. Elles peuvent etre totalement enlevees du program en utilisant le script a la racine de ce projet "setmode.sh"
//          3.1.1. Les deux arguments que ce script prend sont: "debug" et "release". Lorsque vous passez l'argument "debug", ce script ira changer des flags de compilation
//                 que le Makefile utilise pour permettre aux fonction de debogage de faire sortir des messages. Sinon avec "release", les fonctions de debogages n'existeront
//                 pas et elles en fait ne seront meme pas incluses dans le programme, ce qui rend le programme beaucoup moins lourd.
// 4. Essayez de "make clean" toujours avant de faire un "make"/"make install"
// 5. Demandez des questions

//DEFAULT MODE is to READ
volatile uint8_t MODE = READ_MODE;

//COMMANDES DU BYTECODE
enum cmd{DBT=0x01, //commande de depart
         ATT=0x02, //commande d'attente
         DAL=0x44, DET=0x45, //commandes de del: allumer et fermer 
         SGO=0x48, SAR=0x09, //commandes de son: jouer et arreter
         MAR=0x60, MAR2=0x61, MAV=0x62, MRE=0x63, TRD=0x64, TRG=0x65, //commandes de moteurs
         DBC=0xC0, FBC=0xC1, //fonctions de bouclage:
         FIN=0xFF}; //commande de fin

ISR (INT0_vect) {
  variableDelay(30);
  if(PIND & 0x04){
    DEBUG_INFO((uint8_t*)"Button touched!");
    MODE = WRITE_MODE;
  }
  EIFR |= _BV(INTF0);
  cli();
}


int main() {
//Initialization
  uint8_t sreg = SREG;
  cli();
  //PORTS
  DDRB = 0xFF;
  //INTERRUPTS
  EICRA |= _BV(ISC00);
  EIMSK |= _BV(INT0);
  //LIBRARY INITS
  Memoire24CXXX memoire;
  TransmissionUART uart;
  initPWM();
  PIEZO_INIT(DDD4, DDD6);
  sei();
  SREG = sreg;

//Initial Sequence. Interruptable section
  for(uint8_t i=0 ; i<3 ; i++){
    allumerDEL(VERT);
    _delay_ms(1000);
    allumerDEL(ETEIND);
    _delay_ms(1000);
  }
  PLAY_NOTE(45);
  _delay_ms(1000);
  setVolume(0);

//Program

//Choose your program
  
  //Write instructions in...
  if(MODE == WRITE_MODE){
    DEBUG_INFO((uint8_t*)"Entering write mode...");
    allumerDEL(ROUGE);
    for(;;){
      ecrireDonnees(memoire, uart);
    }
  
  //Or read the loaded instructions
  }else if(MODE == READ_MODE){
    uint8_t instruction = 0x00;
    uint8_t operande = 0x00;
    uint16_t address = 0x0000;

    //Setup for loops
    uint8_t boucle_counter = 0x00;
    uint8_t boucle_active = 0x00;
    uint8_t start_loop_address = 0x00;

  //Read bytes 'til you can start (hit 0x01 as the start command)
    do {
      lireDonnees(address, memoire, instruction, operande);
    }while ( instruction != 0x01 );

  //Start reading instructions and operands
    bool finTrouvee = false;
    for (uint16_t i = 0x00; !finTrouvee; i++){
      DEBUG_INFO((uint8_t*)"Entering the loop...");
      lireDonnees(address, memoire, instruction, operande);
      DEBUG_INFO((uint8_t*)"Read the memory...");
      DEBUG_PARAMETER_VALUE((uint8_t*)"instruction",&instruction);
      DEBUG_PARAMETER_VALUE((uint8_t*)"operande",&operande);
      switch (instruction){ 
        case DBT : break;
        case ATT : variableDelay25ms(operande); break;
        case DAL : allumerDEL_dal(operande); break;
        case DET : allumerDEL(ETEIND); break;
        case SGO : setVolume(100); PLAY_NOTE(operande); break;
        case SAR : setVolume(0); break;
        case MAR : arreter(); break;
        case MAR2 : arreter(); break;
        case MAV : avancer(operande); break;
        case MRE : reculer(operande); break;
        case TRD : tournerADroite(); break;
        case TRG : tournerAGauche(); break;
        case DBC : 
          boucle_active = 0x01;
          boucle_counter = operande;
          start_loop_address = address;
          break;
        case FBC :
          if(boucle_active == 0x01){
            if(boucle_counter == 0x00){
              boucle_active = 0x00;
            }else{
              boucle_counter -= 0x01;
            }
            address = start_loop_address;
          }
          break;
        case FIN : finTrouvee = true; break;

      }
    }
  }
  return 0;
}
