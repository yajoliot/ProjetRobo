#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include "pwm.h"
#include "piezo.h"
#include "usart.h"
#include "memoire_24.h"


void lireDonnees(uint16_t& adresse,/*const*/ Memoire24CXXX & memoire, uint8_t & instruction, uint8_t & operande){
  memoire.lecture(adresse, &instruction, sizeof(uint8_t));
  adresse += 0x08;
  memoire.lecture(adresse, &operande, sizeof(uint8_t));
  adresse += 0x08;
}

int main() {

//Initial Sequence
  DDRB=0xFF;
  for(uint8_t i=0 ; i<3 ; i++){
    allumerDEL(VERT);
    _delay_ms(1000);
    allumerDEL(ETEIND);
    _delay_ms(1000);
  }
//Program
  Memoire24CXXX memoire;
  initPWM();
  PIEZO_INIT(DDD4, DDD2);
  PLAY_NOTE(45);
  _delay_ms(1000);
  setVolume(0);
  for(;;){
  uint8_t instruction = 0; // 0 est une valeur non utilisée
  uint8_t operande = 0;
  uint16_t adresse = 0x00;
  // lecture du nombre d'instructions
  uint8_t nbInstructions;
  memoire.lecture(adresse, &nbInstructions, sizeof(uint8_t));
  nbInstructions = (nbInstructions - 0x02) / 0x02;
  adresse += 0x10; //Passer a la premiere instruction

  do {
  memoire.lecture(adresse, &instruction, sizeof(uint8_t));
  // Instruction est sur 8 bits et operande sur 8 bits aussi
  // Pour lire la prochaine instruction il faut sauter 16 bits
  adresse += 0x10;
  nbInstructions--;
  }
  while ( instruction != 0x01 && nbInstructions > 0x00 );

  // Boucle for pour ne pas lire de la memoire qui n'est pas prévue pour ca
  bool finTrouvee = false;
  for (uint16_t i = 0x00; i < nbInstructions && !finTrouvee; i++){
    lireDonnees(adresse, memoire, instruction, operande);
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
        DEBUG_PARAMETER_VALUE((uint8_t*)"operande in SGO", &operande);  
        PLAY_NOTE(operande);
        PLAY_NOTE(45);
        _delay_ms(1000);
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
        break;

      case 0xC1 : // fin de boucle : FBC
        break;

      case 0xFF : // fin : fin
        finTrouvee = true;
        break;

    }
  }
}


  // uint8_t count;
  // uint8_t taille_instructions;
  // for(;;){
  //   taille_instructions = uart.transmissionUART_receive();
  //   for(uint8_t i=0 ; i<taille_instructions ; i++){
  //     memoire.ecriture(i, uart.transmissionUART_receive());
  //   }
  // }


//OLD CODE

  // DDRB = 0xFF;
  // initPWM();
  // PIEZO_INIT(DDD4, DDD2);
  // uint8_t count;
  // for(;;){
    
  //   //MOTOR PWM test
  //   avancer(50);
  //   _delay_ms(3000);
  //   arreter();
  //   tournerADroite();
  //   tournerAGauche();
  //   arreter();
  // //these functions cover nearly everything. reculer() wasn't called but same idea as avancer().

  // //PIEZO test
  //   count=45;
  //   setVolume(100);
  //   for(;count<81;count++){
  //     PLAY_NOTE(count);
  //   }
  //   setVolume(0);
  // }


  return 0;
}