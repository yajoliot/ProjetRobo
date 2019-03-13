#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>


const uint8_t Eteinte = 0x00;
const uint8_t Rouge = 0x01; 
const uint8_t Verte = 0x02;
const uint8_t DelaiMs = 10;
const uint8_t MaximumVert = 85; /*seuil maximum pour passer du vert au ambre,defini sur une 
plage de 0 a 255,vraiment on peut choisir le nombre voulu ,ici on choisit 255/3 car on a 3 etats*/
const uint8_t MinimumRouge = 170; /* seuil minimum pour passer de l'ambre au rouge, 
choisi arbitrairement entre MaximumVert et 255 , on choisit ici 2*(255/3)*/
 					   
int main (){
	
	DDRA = 0x00; //port A en entree
	DDRB = 0xFF; //port B en sortie
	can Convertion; // Convertion est un objet de type "can"
	//Debut de la boucle infinie
	for(;;) {
		uint8_t Seuil = (Convertion.lecture (1) >> 2); /*faire la lecture,puis 
        decaler de deux bits vers la droite car les bits les moins significatifs ne sont pas utilises.*/
		if (Seuil <= MaximumVert) //si la lumiere est faible, la DEL est verte
			PORTB = Verte;	
    //boucle pour que la DEL soit ambre, soit une alternance rapide entre rouge et vert
		else if (Seuil > MaximumVert && Seuil < MinimumRouge) {
			PORTB = Rouge; //DEL rouge
			_delay_ms (10); 
			PORTB = Verte; //DEL verte
			_delay_ms (10);
		} else 	PORTB = Rouge; /*si l'intensite lumineuse est trop elevee, la DEl 
        est rouge*/
	}
 return 0;
}