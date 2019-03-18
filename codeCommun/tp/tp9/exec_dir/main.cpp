#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include "pwm.h"
#include "piezo.h"
#include "usart.h"
#include "memoire_24.h"
#include "usart.h"

const uint8_t WRITE_MODE = 0x00;
const uint8_t READ_MODE = 0x01;
//DEFAULT MODE is to READ
volatile uint8_t MODE = READ_MODE;

void lireDonnees(uint16_t& address, Memoire24CXXX &_memoire, uint8_t & instruction, uint8_t & operande){
  // Memoire24CXXX _memoire;
  _memoire.lecture(address, &instruction, sizeof(uint8_t));
  address += 0x01;
  _memoire.lecture(address, &operande, sizeof(uint8_t));
  address += 0x01;
}

void ecrireDonnees(Memoire24CXXX &_memoire, TransmissionUART &_uart){
  uint8_t taille_octet_1 = _uart.transmissionUART_receive();
  uint8_t taille_octet_2 = _uart.transmissionUART_receive();
  uint16_t taille_octet_total = 0x00;
  taille_octet_total |= (taille_octet_1 << 8) | taille_octet_2;
  for (uint16_t i = 0; i < taille_octet_total;i++)
  {
    _memoire.ecriture(i, _uart.transmissionUART_receive());
  }
}

ISR (INT0_vect) {
  variableDelay(30);
  if(PIND & 0x04){
    MODE = WRITE_MODE;
  }
  EIFR |= _BV(INTF0);
}


int main() {
//Initialization
  cli();
  EICRA |= _BV(ISC00);
  EIMSK |= _BV(INT0);
  Memoire24CXXX memoire;
  TransmissionUART uart;
  initPWM();
  PIEZO_INIT(DDD4, DDD2);
  sei();

//Initial Sequence
  DDRB=0xFF;
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
    ecrireDonnees(memoire, uart);
  
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
      memoire.lecture(address, &instruction, sizeof(uint8_t));
      address += 0x02;
    }while ( instruction != 0x01 );

  //Start reading instructions and operands
    bool finTrouvee = false;
    for (uint16_t i = 0x00; !finTrouvee; i++){
      DEBUG_INFO((uint8_t*)"WE IN THE LOOP WOOHOO");
      lireDonnees(address, memoire, instruction, operande);
      DEBUG_INFO((uint8_t*)"MANAGED TO READ MEM!!!");
      DEBUG_PARAMETER_VALUE((uint8_t*)"instruction",&instruction);
      DEBUG_PARAMETER_VALUE((uint8_t*)"operande",&operande);
      switch (instruction){ 
        case 0x02 : // attendre att
          for (uint16_t i = 0; i < operande; i ++){
            _delay_ms(25);
          }
          break;

        case 0x44 : // allumer les DEL : dal
          // Possibilite d'allumer la DEL en rouge ou en vert en fonction de l'operande
          if (operande == 0x01){ // Allume en vert
            allumerDEL(VERT);
          }
          else { // Allume en rouge
            allumerDEL(ROUGE);
          }
          
          break;

        case 0x45 : // eteindre les DEL : det
          PORTB &= 0xFC;
          break;
        
        case 0x48 : // jouer une sonorite : SGO  
          setVolume(100);
          PLAY_NOTE(operande);
          break;

        case 0x09 : // arreter de jouer la sonorite en cours : SAR
          setVolume(0);
          break;

        case 0x60 : // arreter les moteurs : MAR
          arreter();
          break;

        case 0x61 : // arreter les moteurs : MAR
          arreter();
          break;

        case 0x62 : // avancer : MAV
          avancer(operande);
          break;
        
        case 0x63 : // reculer : MRE
          reculer(operande);
          break;

        case 0x64 : // Trouner a droite : TRD
          tournerADroite();
          break;

        case 0x65 : // tourner a gauhce : TRG
          tournerAGauche();
          break;
        
        case 0xC0 : // debut de boucle : DBC
          boucle_active = 0x01;
          boucle_counter = operande;
          start_loop_address = address;
          break;

        case 0xC1 : // fin de boucle : FBC
          if(boucle_active == 0x01){
            if(boucle_counter == 0x00){
              boucle_active = 0x00;
            }else{
              boucle_counter -= 0x01;
            }
            address = start_loop_address;
          }
          break;

        case 0xFF : // fin : fin
          finTrouvee = true;
          break;

      }
    }
  }
  return 0;
}
