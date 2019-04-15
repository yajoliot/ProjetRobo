#ifndef LIB_DIR_PWM_H
#define LIB_DIR_PWM_H
#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"
#include "Minuterie.h"



class PWM{
    private:
        
        //Constantes d'ajustement des roues afin de faire en sorte que le robot
        //avance en ligne droite
        static const uint8_t AJUSTEMENT_DROITE = 2;
        static const uint8_t AJUSTEMENT_GAUCHE = 4;

        //Diminution minimale possible d'une roue pour ajuster sa trajectoire
        static const uint8_t DIMINUTION_MINIMALE = 80;

        static const uint8_t DIMINUTION_REDUCTION = 80;
        
        //Vitesse maximale
        static const uint8_t VITESSE_MAX = 255;

        //Vitesse par defaut
        uint8_t VITESSE_DEFAULT = 135;
        
        
        uint8_t vitesseTournage = 130;

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
        uint8_t getVitesseTournage();

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
        void tournantDroite(uint8_t &rapport, uint8_t valueMap);
        void boite(uint8_t &rapport, uint8_t valueMap);
        void ajustementADroite ();
        void ajustementAGauche();
        void ajustementReculerAGauche();
        void ajustementReculerADroite();
        void tourner90Gauche(uint8_t rapport);
        void tourner90Droite(uint8_t rapport);
        
        

        void ralentissementGauche(uint8_t &rapport, uint8_t valueMap);
        void rotateDroite(uint8_t rapport);
        void rotateGauche(uint8_t rapport);
        //section1

        void avancerTimer(uint8_t valeur, uint32_t timer);
        void tourner90Precis(uint8_t direc, uint8_t rapport);



};


#endif

