#include "Graphique/libgraph.h"
#include "ListeChainee/liste.c"
#include <SDL/SDL.h>



void creerLigneMechant(listeEntites* l, int y, int sprite) {
	int longueur, largeur;
	tailleLutin(sprite, &longueur, &largeur);
	entite e = {0, y, sprite};
	for (int i=0 ; i<11 ; i++) {
		e.x = 2*longueur*i;
		addHead(e, l);
	}
}



void ajouterEntite(listeEntites* l, int x, int y, int sprite) {
	entite e = {x, y, sprite};
	addHead(e, l);
}



void afficherListe(listeEntites l) {
	while (l != NULL) {
		afficherLutin(l->val.sprite, l->val.x, l->val.y);
		l = l->next;
	}
}



void mvt_droite(listeEntites* l, int* direction) {	
	int longueur, largeur;
	tailleLutin((*l)->val.sprite, &longueur, &largeur);

	if (*l != NULL && (*l)->val.x <= (1280-longueur/2)-100) {
		listeEntites p = *l;
		while (p != NULL) {
			p->val.x += 5;
			p = p->next;
		}
 	} else {
		*direction = -1;
		listeEntites p = *l;
		while (p != NULL) {
			p->val.y += largeur;
			p = p->next;
		}
	}	
}



void mvt_gauche(listeEntites* l, int* direction) {	
	int longueur, largeur;
	tailleLutin((*l)->val.sprite, &longueur, &largeur);

	listeEntites h = *l;
	while(h != NULL && h->next != NULL) {
		h = h->next;
	}
	if (h->val.x >= (0+longueur/2)+100) {
		listeEntites p = *l;
		while (p != NULL) {
			p->val.x -= 5;
			p = p->next;
		}
 	} else {
		*direction = 1;
		listeEntites p = *l;
		while (p != NULL) {
			p->val.y += largeur;
			p = p->next;
		}

	}	
}




int main() {
	char titre[25] = "Space_Invaders";
	creerSurface(1280, 800, titre);


	
	int monstre_1_1 = chargerLutin("Lutins/invader_monstre1_1.bmp", COULEUR_NOIR);
	//int monstre_1_2 = chargerLutin("Lutins/invader_monstre1_2.bmp", COULEUR_NOIR);
	//int monstre_2_1 = chargerLutin("Lutins/invader_monstre2_1.bmp", COULEUR_NOIR);
	//int monstre_2_2 = chargerLutin("Lutins/invader_monstre2_2.bmp", COULEUR_NOIR);
	//int monstre_3_1 = chargerLutin("Lutins/invader_monstre3_1.bmp", COULEUR_NOIR);
	//int monstre_3_2 = chargerLutin("Lutins/invader_monstre3_2.bmp", COULEUR_NOIR);
	//int canon = chargerLutin("Lutins/invader_canon.bmp", COULEUR_NOIR);
	

	listeEntites listeMechant = NULL;
	creerLigneMechant(&listeMechant, 0, monstre_1_1);
	int direction = 1;

	afficherListe(listeMechant);
	majSurface();
	SDL_Delay(1000);


	for (int i=0 ; i<200 ; i++) {
		rectanglePlein(0, 0, 1280, 800, COULEUR_NOIR);
		if (direction == 1) mvt_droite(&listeMechant, &direction);
		else mvt_gauche(&listeMechant, &direction);
		afficherListe(listeMechant);
		SDL_Delay(100);
		majSurface();
	}


   

  	fermerSurface();
  	return 0;
}
