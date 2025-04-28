#include "Graphique/libgraph.h"
#include "ListeChainee/liste.c"
#include <SDL/SDL.h>

void creerMechant(int *sprite, int nbL, int nbC, int xO, int yO, listeEntites *l) {
	entite = e;
	int l, h;
	tailleLutin(sprite, l, h);
	for(int i=0 ; i<nbL ; i++) {
		for(int j=0 ; j<nbC ; j++) {
		e.x = xO+(i*l);
		e.y = yO+(j*h);
		e.sprite = sprite;
		addHead(e, l*);
		}
	}


void afficheMechant() {
}



int main() {
	char titre[25] = "Space_Invaders";
	creerSurface(1000, 1000, titre);

	ListeMechant = makeList(NULL, NULL);
	creerMechant("Lutins/invader_monstre1_1.bmp", 1, 12, 10, 10, &ListeMechant);
	
	majSurface();
	SDL_Delay(3000);
  

  	fermerSurface();
  	return 0;
}
