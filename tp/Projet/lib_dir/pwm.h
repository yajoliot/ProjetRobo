#ifndef LIB_DIR_PWM_H
#define LIB_DIR_PWM_H
#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"




class PWM{
    private:
        const uint8_t AJUSTEMENT = 7;
        const uint8_t VITESSE_MAX = 255;
        uint8_t VITESSE_DEFAULT = 150;
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
        uint8_t getVitesseDefault();

        void initPWM();
        void roueGauche(bool direction, uint8_t rapport);
        void roueDroite(bool direction,uint8_t rapport);
        void avancer(uint8_t rapport);
        void avancementAjuste(uint8_t &rapport, uint8_t valueMap);
        void reculer(uint8_t rapport);
        void arreter();
        void tournerADroite();
        void tournerAGauche();
        void tournantGauche(uint8_t &rapport, uint8_t valueMap);
        void boite(uint8_t &rapport, uint8_t valueMap);
        void ajustementADroite ();
        void ajustementAGauche();
        void ajustementReculerAGauche();
        void ajustementReculerADroite();

};


#endif
