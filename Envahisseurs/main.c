#include "Graphique/libgraph.h"
#include "ListeChainee/liste.c"
#include <SDL/SDL.h>
#include <stdlib.h>


#define LONGUEUR 1280
#define LARGEUR 800



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

	if (*l != NULL && (*l)->val.x <= (LONGUEUR-longueur)) {
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
	if (h->val.x >= 0) {
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



void ajouter_bombe(listeEntites l, listeEntites* b, int sprite) {
	entite e = {0, 0, sprite};
	while(l != NULL) {
		if (rand()%1001 < 3) {	
			e.x = l->val.x;
			e.y = l->val.y;
			addHead(e, b);
		}
		l = l->next;
	}

}



void deplacer_bombe(listeEntites* b) {
	if (*b != NULL) {
		if ((*b)->val.y > LARGEUR) {
			deleteHead(b);
		} else { 
			(*b)->val.y += 5;
			deplacer_bombe(&(*b)->next);
		}
	}
}



void afficher_joueur (entite e) {
	afficherLutin(e.sprite, e.x, e.y);
}



int deplacer_joueur_quitter(evenement evt, char touche, entite* e) {
	int largeur;
	int hauteur;
	tailleLutin(e->sprite, &largeur, &hauteur);
	if (evt == toucheBas) {
		if (touche == 'd' && (e->x)+5 < LONGUEUR-largeur) {
			e->x += 5;
		} else if (touche == 'q' && (e->x)-5 > 0) {
			e->x -=5;
		}
	}
	if (evt == quitter) {
		return 0;
	} else {
		return 1;
	}
}





	


int main() {
	char titre[25] = "Space_Invaders";
	creerSurface(LONGUEUR, LARGEUR, titre);


	
	int monstre_1_1 = chargerLutin("Lutins/invader_monstre1_1.bmp", COULEUR_NOIR);
	//int monstre_1_2 = chargerLutin("Lutins/invader_monstre1_2.bmp", COULEUR_NOIR);
	//int monstre_2_1 = chargerLutin("Lutins/invader_monstre2_1.bmp", COULEUR_NOIR);
	//int monstre_2_2 = chargerLutin("Lutins/invader_monstre2_2.bmp", COULEUR_NOIR);
	//int monstre_3_1 = chargerLutin("Lutins/invader_monstre3_1.bmp", COULEUR_NOIR);
	//int monstre_3_2 = chargerLutin("Lutins/invader_monstre3_2.bmp", COULEUR_NOIR);
	int canon = chargerLutin("Lutins/invader_canon.bmp", COULEUR_NOIR);
	int bombe = chargerLutin("Lutins/invader_bombe.bmp", COULEUR_NOIR);
	

	listeEntites listeMechant = NULL;
	listeEntites listeBombe = NULL;
	entite joueur = {LONGUEUR / 2, LARGEUR - 100, canon};
	creerLigneMechant(&listeMechant, 0, monstre_1_1);
	int direction = 1;
	evenement evt;
	char touche;
	int jouer = 1;

	afficherListe(listeMechant);
	afficher_joueur(joueur);
	majSurface();
	SDL_Delay(1000);


	while (jouer) {
		rectanglePlein(0, 0, 1280, 800, COULEUR_NOIR);
		if (direction == 1) mvt_droite(&listeMechant, &direction);
		else mvt_gauche(&listeMechant, &direction);
		deplacer_bombe(&listeBombe);
		ajouter_bombe(listeMechant, &listeBombe, bombe);
		lireEvenement(&evt, &touche, NULL);
		jouer = deplacer_joueur_quitter(evt, touche, &joueur);

		afficherListe(listeMechant);
		afficherListe(listeBombe);
		afficher_joueur(joueur);
		SDL_Delay(15);
		majSurface();
	}


   

  	fermerSurface();
  	return 0;
}
