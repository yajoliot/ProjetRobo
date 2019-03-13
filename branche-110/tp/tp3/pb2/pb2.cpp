#define F_CPU 8000000
#define INTERRUPTEUR_ON (PIND & 0x04)
#include <stdint.h>
#include <avr/io.h> 
#include <util/delay.h>

//le temps pendant lequel la roue tourne, soit 20 secondes.

void PWM(){
    double puissance;
    double a ; 
    double b ; // Ceci correspond a la periode
    double frequence;
    frequence=60.0;
    a=0.0;
    b=1.0/frequence;
    while(a <= b){ //boucle pour une frequence de 60HZ
          puissance=a/b;
          _delay_ms(2000);//faire rouler a puissance pendant 2s,soit temps/10 mais je ne sais pas si sa marche.La boucle en dessous pourrait mieux marcher
          a=a+b/4; // augmenter la valeur de a par 1/4 de b pour une augmentation de puissance de 25% a la fois.
        }
    frequence=400.0;
    b=1/frequence;
    while(a <= b){ //boucle pour une frequence de 400HZ
          puissance=a/b;
          _delay_ms(2000);//faire rouler a puissance pendant 2s,je ne suis pas sur;
          a=a+b/4; // augmenter la valeur de a par 1/4 de b pour une augmentation de puissance de 25% a la fois.
        }
    
    }
int main(){
    PORTB=0B00000011;
}
