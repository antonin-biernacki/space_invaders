#include "Graphique/libgraph.h"
#include "ListeChainee/liste.c"
#include <SDL/SDL.h>
#include <stdlib.h>


#define LONGUEUR 1280 //1280
#define LARGEUR 800 //800
#define DELAI_MISSILE 10 // delai entre 2 missiles


/* Gestion des listes d'entites */

void creer_ligne_mechant(listeEntites* l, int y, int sprite, int petit_sprite) {
	int longueur, largeur;
	tailleLutin(sprite, &longueur, &largeur);
	entite e = {0, y, sprite};	
	if (petit_sprite == 0) {
		for (int i=0 ; i<11 ; i++) {
			e.x = 2*i*longueur;
			addHead(e, l);
		}
	} else {
		for (int i=0 ; i<12 ; i++) {
			e.x = 2*i*(longueur+1.8);
			addHead(e, l);
		}
	}
}



void creer_ligne_bouclier(listeEntites* l, int y, int sprite) {
	int longueur, largeur;
	tailleLutin(sprite, &longueur, &largeur);
	entite e = {0, y, sprite};
	for(int i=0 ; i<3 ; i++) {
		e.x = i*longueur + (i+1)*(LONGUEUR/4-50);
		addHead(e, l);
	}
}



void descendre_invaders(listeEntites* l) {
	listeEntites p = *l;
	while (p != NULL && *l != NULL) {
		p->val.y += 15;
		p = p->next;
	}

}



void mvt_droite(listeEntites* l1, listeEntites* l2, listeEntites* l3, int* direction) {	
	int longueur, largeur;
	if (*l1 != NULL) {
		tailleLutin((*l1)->val.sprite, &longueur, &largeur);

		if ((*l1)->val.x <= (LONGUEUR-longueur)) {
			listeEntites p = *l1;
			while (p != NULL) {
				p->val.x += 2;
				p = p->next;
			}
 		} else {
			if (*direction == -1) {
				descendre_invaders(l1);
				descendre_invaders(l2);
				descendre_invaders(l3);
			}
			*direction = -1;
		}	
	}
}



void mvt_gauche(listeEntites* l1, listeEntites* l2, listeEntites* l3, int* direction) {	
	int longueur, largeur;
	if (*l1 != NULL) {
		tailleLutin((*l1)->val.sprite, &longueur, &largeur);

		listeEntites h = *l1;
		while(h != NULL && h->next != NULL) {
			h = h->next;
		}
		if (h->val.x >= 0) {
			listeEntites p = *l1;
			while (p != NULL) {
				p->val.x -= 2;
				p = p->next;
			}
 		} else {
			if (*direction == 1) { 
				descendre_invaders(l1);
				descendre_invaders(l2);
				descendre_invaders(l3);
			}
			*direction = 1;
		}	
	}
}



void ajouter_bombe(listeEntites l, listeEntites* b, int sprite) {
	entite e = {0, 0, sprite};
	while(l != NULL) {
		if (rand()%1001 < 3) {  //rand()%1001 < 3	
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



void ajouter_missile(evenement evt, char touche, listeEntites* missile, entite joueur, int sprite, int* delai) {
	if (*delai < 0 && evt == toucheBas && touche == 'z') {
		int longeur, largeur;
		tailleLutin(joueur.sprite, &longeur, &largeur);
		entite e = {joueur.x + longeur/2 - 4, joueur.y - largeur/2, sprite};
		addHead(e, missile);
		*delai = DELAI_MISSILE;
	} else {
		(*delai)--;
	}
}



void deplacer_missile(listeEntites* missile) {
	if (*missile != NULL) {
		if ((*missile)->val.y < 0) {
			deleteHead(missile);
		} else { 
			(*missile)->val.y -= 10;
			deplacer_missile(&(*missile)->next);
		}
	}
}

/* Gestion joueur */

void deplacer_joueur(evenement evt, char touche, entite* e) {
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
}

/* Gestion collisions */

int collision_entite_entite(entite e1, entite e2) {
	int longueur1, largeur1, longueur2, largeur2;
	tailleLutin(e1.sprite, &longueur1, &largeur1);
	tailleLutin(e2.sprite, &longueur2, &largeur2);
	if ((e1.x >= e2.x && e1.x <= (e2.x + longueur2)) || ((e1.x + longueur1) >= e2.x && (e1.x + longueur1) <= (e2.x + longueur2)) || (e2.x >= e1.x && e2.x <= (e1.x + longueur1)) || ((e2.x + longueur2) >= e1.x && (e2.x + longueur2) <= (e1.x + longueur1))) {
		if ((e1.y >= e2.y && e1.y <= (e2.y + largeur2)) || ((e1.y + largeur1) >= e2.y && (e1.y + largeur1) <= (e2.y + largeur2)) || (e2.y >= e1.y && e2.y <= (e1.y + largeur1)) || ((e2.y + largeur2) >= e1.y && (e2.y + largeur2) <= (e1.y + largeur1))) {
			return 1;
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}




int collision_entite_liste(listeEntites* l, entite e) {
	if (*l != NULL) {
		if (collision_entite_entite((*l)->val, e) == 1) {
			deleteHead(l);
			return 1;
		} else {
			return collision_entite_liste(&(*l)->next, e);
		}
	} else {
		return 0;
	}
}


 
void collision_liste_liste(listeEntites* l1, listeEntites* l2) {
	if (*l1 != NULL && *l2 != NULL) {
		if (collision_entite_liste(l1, (*l2)->val) == 1) {
			deleteHead(l2);
			collision_liste_liste(l1, l2);
		} else {
			collision_liste_liste(l1, &(*l2)->next);
		}
	}
}

/* Divers */

int quitter_jeu(evenement evt) {
	if (evt == quitter) {
		return 0;
	} else {
		return 1;
	}
}



int perdre(listeEntites* l1, listeEntites* l2, listeEntites* l3, entite joueur) {
	if ((collision_entite_liste(l1, joueur)) || (collision_entite_liste(l2, joueur)) || (collision_entite_liste(l3, joueur))) {
		return 0;
	} else if (*l1 != NULL && (*l1)->val.y >= LARGEUR-100) {
	 	return 0;
	} else if (*l2 != NULL && (*l2)->val.y >= LARGEUR-100) {
	 	return 0;
	} else if (*l3 != NULL && (*l3)->val.y >= LARGEUR-100) {
	 	return 0;
	} else {
	      	return 1;
	}	
}



int gagner(listeEntites l1, listeEntites l2, listeEntites l3) {
	return !(l1 == NULL && l2 == NULL && l3 == NULL);
}

/* Affichage */

void afficher_liste(listeEntites l) {
	while (l != NULL) {
		afficherLutin(l->val.sprite, l->val.x, l->val.y);
		l = l->next;
	}
}



void afficher_joueur (entite e) {
	afficherLutin(e.sprite, e.x, e.y);
}



void afficher_vie (int nb_vie, int vies, int vie1, int vie2, int vie3) {
	afficherLutin(vies, LONGUEUR/2-100, 0);
	if (nb_vie == 3) {
		afficherLutin(vie3, LONGUEUR/2+80, 0);
	} else if (nb_vie == 2) {
		afficherLutin(vie2, LONGUEUR/2+80, 0);
	} else {
		afficherLutin(vie1, LONGUEUR/2+80, 0);
	}
}


/* Main */

int main() {
	// Creation de la fenetre
	char titre[25] = "Space_Invaders";
	creerSurface(LONGUEUR, LARGEUR, titre);


	// Chargement des lutins
	int monstre_1_1 = chargerLutin("Lutins/invader_monstre1_1.bmp", COULEUR_NOIR);
	//int monstre_1_2 = chargerLutin("Lutins/invader_monstre1_2.bmp", COULEUR_NOIR);
	int monstre_2_1 = chargerLutin("Lutins/invader_monstre2_1.bmp", COULEUR_NOIR);
	//int monstre_2_2 = chargerLutin("Lutins/invader_monstre2_2.bmp", COULEUR_NOIR);
	int monstre_3_1 = chargerLutin("Lutins/invader_monstre3_1.bmp", COULEUR_NOIR);
	//int monstre_3_2 = chargerLutin("Lutins/invader_monstre3_2.bmp", COULEUR_NOIR);
	int canon = chargerLutin("Lutins/invader_canon.bmp", COULEUR_NOIR);
	int bouclier = chargerLutin("Lutins/invader_bouclier.bmp", COULEUR_NOIR);
	int bombe = chargerLutin("Lutins/invader_bombe.bmp", COULEUR_NOIR);
	int missile = chargerLutin("Lutins/invader_missile.bmp", COULEUR_NOIR);
	int vies = lutinTexte("Nombre de vies :", COULEUR_BLANC);
	int vie1 = lutinTexte("1", COULEUR_BLANC);
	int vie2 = lutinTexte("2", COULEUR_BLANC);
	int vie3 = lutinTexte("3", COULEUR_BLANC);
	int victoire = lutinTexte("gg", COULEUR_BLANC);
	int defaite = lutinTexte("nul", COULEUR_BLANC);


	
	// Initialisation
	listeEntites listeMechant1 = NULL;
	listeEntites listeMechant2 = NULL;
	listeEntites listeMechant3 = NULL;
	listeEntites listeBombe = NULL;
	listeEntites listeMissile = NULL;
	listeEntites listeBouclier = NULL;
	entite joueur = {LONGUEUR / 2 - 40, LARGEUR - 100, canon};
	
	creer_ligne_mechant(&listeMechant1, 50, monstre_1_1, 1);
	creer_ligne_mechant(&listeMechant2, 100, monstre_2_1, 0);
	creer_ligne_mechant(&listeMechant3, 150, monstre_3_1, 0);
	creer_ligne_bouclier(&listeBouclier, LARGEUR-180, bouclier);

	int direction = 1; // Permet de savoir dans quel sens se deplace les invaders
	evenement evt;
	char touche;
	int jouer = 1; // Permet de savoir si le jeu est fini ou quitter
	int perdu = 1;
	int gagne = 1;
	int delai = 0; // Permet de gerer le delai entre 2 missiles
	int nb_vie = 3;

	afficher_liste(listeMechant1);
	afficher_liste(listeMechant2);
	afficher_liste(listeMechant3);
	afficher_joueur(joueur);
	majSurface();
	SDL_Delay(1000);


	while (jouer) {
		rectanglePlein(0, 0, LONGUEUR, LARGEUR, COULEUR_NOIR);
		lireEvenement(&evt, &touche, NULL);
		jouer = quitter_jeu(evt);


		if (perdu && gagne && nb_vie > 0) {	
			if (direction == 1) {
		       		mvt_droite(&listeMechant1, &listeMechant2, &listeMechant3, &direction);
				mvt_droite(&listeMechant2, &listeMechant1 ,&listeMechant3, &direction);
				mvt_droite(&listeMechant3, &listeMechant1, &listeMechant2, &direction);

			} else {
				mvt_gauche(&listeMechant1, &listeMechant2, &listeMechant3, &direction);
				mvt_gauche(&listeMechant2, &listeMechant1, &listeMechant3, &direction);
				mvt_gauche(&listeMechant3, &listeMechant1, &listeMechant2, &direction);
			}

			deplacer_bombe(&listeBombe);
			deplacer_missile(&listeMissile);

			deplacer_joueur(evt, touche, &joueur);
			ajouter_bombe(listeMechant3, &listeBombe, bombe);
			ajouter_missile(evt, touche, &listeMissile, joueur, missile, &delai);

			if (collision_entite_liste(&listeBombe, joueur)) {
				nb_vie--;
			}
			collision_liste_liste(&listeMechant1, &listeMissile);
			collision_liste_liste(&listeMechant2, &listeMissile);
			collision_liste_liste(&listeMechant3, &listeMissile);
			collision_liste_liste(&listeBombe, &listeMissile);
			collision_liste_liste(&listeBouclier, &listeBombe);
			collision_liste_liste(&listeBouclier, &listeMissile);



			perdu = perdre(&listeMechant1, &listeMechant2, &listeMechant3, joueur);
			gagne = gagner(listeMechant1, listeMechant2, listeMechant3);


			afficher_liste(listeMechant1);
			afficher_liste(listeMechant2);
			afficher_liste(listeMechant3);
			afficher_liste(listeBombe);
			afficher_liste(listeMissile);
			afficher_liste(listeBouclier);
			afficher_joueur(joueur);
			afficher_vie(nb_vie, vies, vie1, vie2, vie3);

		} else {
			if (!gagne) {
				afficherLutin(victoire, LONGUEUR/2, LARGEUR/2);		
			}
			if (!perdu || nb_vie <= 0) {
				afficherLutin(defaite, LONGUEUR/2, LARGEUR/2);
			}
		}


		
		SDL_Delay(15);
		majSurface();
	}

	

   

  	fermerSurface();
  	return 0;
}
