#include "bytecode.h"

void lireDonnees(uint16_t& address, Memoire24CXXX &_memoire, uint8_t & instruction, uint8_t & operande){
  // Memoire24CXXX _memoire;
  _memoire.lecture(address, &instruction, sizeof(uint8_t));
  address += 0x01;
  _memoire.lecture(address, &operande, sizeof(uint8_t));
  address += 0x01;
}

void ecrireDonnees(Memoire24CXXX &_memoire, TransmissionUART &_uart){
  uint16_t taille_octet_1 = _uart.transmissionUART_receive();
  uint16_t taille_octet_2 = _uart.transmissionUART_receive();
  uint16_t taille_octet_total = 0x00;
  taille_octet_total |= (taille_octet_1 << 8) | taille_octet_2;
  for (uint16_t i = ADDRESSE_INITIALE; i < taille_octet_total;i++)
  {
    _memoire.ecriture(i, _uart.transmissionUART_receive());
  }
}