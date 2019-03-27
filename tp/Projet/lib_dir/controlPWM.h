#ifndef LIB_DIR_ANTIREBOND_H
#define LIB_DIR_ANTIREBOND_H

#include <avr/io.h> 
#include <util/delay.h>


Class ControlPWM{
    private:

        uint8_t rapportGauche,
        rapportDroite,
        directionGauche,
        directionDroite;
        
        
    public:
        ControlPWM();

        uint8_t getRapport();
        uint8_t getDirection();

        void initPWM();
        void roueGauche(bool direction, uint8_t rapport);
        void roueDroite(bool direction,uint8_t rapport);
        void avancer(uint8_t rapport);
        void reculer(uint8_t rapport);
        void arreter();
        void tournerADroite();
        void tournerAGauche();
        void ajustementADroite ();
        void ajustementAGauche();
        void ajustementReculerAGauche();
        void ajustementReculerADroite();

}


#endif LIB_DIR_ANTIREBOND_H
