#ifndef LIB_DIR_PWM_H
#define LIB_DIR_PWM_H
#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>


class PWM{
    private:

        uint8_t rapportGauche,
        rapportDroite,
        directionGauche,
        directionDroite;
        
        
    public:
        PWM();

        uint8_t getRapportDroite();
        uint8_t getRapportGauche();
        uint8_t getDirectionGauche();
        uint8_t getDirectionDroite();

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

};


#endif
