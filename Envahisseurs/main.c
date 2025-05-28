#include "Graphique/libgraph.h"
#include "ListeChainee/liste.h"
#include <SDL/SDL.h>
#include <stdlib.h>


#define LONGUEUR 1280 // taille x de la fenetre 
#define LARGEUR 800 // taille y de la fenetre
#define DELAI_MISSILE 10 // delai entre 2 missiles


/* Gestion des listes d'entites */

void creer_ligne_mechant(listeEntites* l, int y, int sprite, int petit_sprite) {
	int longueur, largeur, nb_invaders;
	tailleLutin(sprite, &longueur, &largeur);
	entite e = {0, y, sprite};	
	if (petit_sprite == 0) {
		nb_invaders = 11;
		for (int i=0 ; i<nb_invaders ; i++) {
			e.x = 2*i*longueur;
			addHead(e, l);
		}
	} else {
	nb_invaders = 12;
	int ajuste_ecart = 1.8;
		for (int i=0 ; i<nb_invaders ; i++) {
			e.x = 2*i*(longueur+ajuste_ecart);
			addHead(e, l);
		}
	}
}



void creer_ligne_bouclier(listeEntites* l, int y, int sprite) {
	int longueur, largeur, nb_bouclier;
	tailleLutin(sprite, &longueur, &largeur);
	entite e = {0, y, sprite};
	nb_bouclier = 3;
	int distance = 50;
	for(int i=0 ; i<nb_bouclier ; i++) {
		e.x = i*longueur + (i+1)*(LONGUEUR/(nb_bouclier+1)-distance);
		addHead(e, l);
	}
}



void descendre_invaders(listeEntites* l) {
	listeEntites p = *l;
	int distance = 15;
	while (p != NULL && *l != NULL) {
		p->val.y += distance;
		p = p->next;
	}

}



void mvt_invaders(listeEntites* l1, listeEntites* l2, listeEntites* l3, int* direction) {	
	int longueur, largeur;
	int distance = 2;
	if (*l1 != NULL) {
		tailleLutin((*l1)->val.sprite, &longueur, &largeur);
		listeEntites h = *l1;
		while(h != NULL && h->next != NULL) {
			h = h->next;
		}


		if (*direction == 1 && (*l1)->val.x <= (LONGUEUR-longueur)) {
			listeEntites p = *l1;
			while (p != NULL) {
				p->val.x += distance;
				p = p->next;
			}

		} else if (*direction == 0 && h->val.x >= 0) {
			listeEntites p = *l1;
			while (p != NULL) {
				p->val.x -= distance;
				p = p->next;
			}

		} else {
			*direction = !(*direction);
			descendre_invaders(l1);
			descendre_invaders(l2);
			descendre_invaders(l3);

		}
	}
}



void ajouter_bombe(listeEntites l, listeEntites* b, int sprite) {
	entite e = {0, 0, sprite};
	int div = 1000;
	int proba = 0.5;
	while(l != NULL) {
		if (rand()%div < proba) {  //rand()%1001 < 3	
			e.x = l->val.x;
			e.y = l->val.y;
			addHead(e, b);
		}
		l = l->next;
	}

}



void deplacer_bombe(listeEntites* b) {
	int distance = 5;
	if (*b != NULL) {
		if ((*b)->val.y > LARGEUR) {
			deleteHead(b);
		} else { 
			(*b)->val.y += distance;
			deplacer_bombe(&(*b)->next);
		}
	}
}



void ajouter_missile(evenement evt, char touche, listeEntites* missile, entite joueur, int sprite, int* delai) {
	int ajuste_distance = 4;
	if (*delai < 0 && evt == toucheBas && touche == 'z') {
		int longeur, largeur;
		tailleLutin(joueur.sprite, &longeur, &largeur);
		entite e = {joueur.x + longeur/2 - ajuste_distance, joueur.y - largeur/2, sprite};
		addHead(e, missile);
		*delai = DELAI_MISSILE;
	} else {
		(*delai)--;
	}
}



void deplacer_missile(listeEntites* missile) {
	int distance = 10;
	if (*missile != NULL) {
		if ((*missile)->val.y < 0) {
			deleteHead(missile);
		} else { 
			(*missile)->val.y -= distance;
			deplacer_missile(&(*missile)->next);
		}
	}
}

/* Gestion joueur */

void deplacer_joueur(evenement evt, char touche, entite* e) {
	int distance = 5;
	int largeur, hauteur;
	tailleLutin(e->sprite, &largeur, &hauteur);
	if (evt == toucheBas) {
		if (touche == 'd' && (e->x)+distance < LONGUEUR-largeur) {
			e->x += distance;
		} else if (touche == 'q' && (e->x)-distance > 0) {
			e->x -= distance;
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

void mettre_pause(evenement evt, char touche, int* pause) {
	if (evt == toucheBas && touche == 'p') {
		*pause = !(*pause);
	}
}



int gagner(listeEntites l1, listeEntites l2, listeEntites l3) {
	return !(l1 == NULL && l2 == NULL && l3 == NULL);
}



int perdre(listeEntites* l1, listeEntites* l2, listeEntites* l3, entite joueur) {
	int distance_joueur = 100;
	if ((collision_entite_liste(l1, joueur)) || (collision_entite_liste(l2, joueur)) || (collision_entite_liste(l3, joueur))) {
		return 0;
	} else if (*l1 != NULL && (*l1)->val.y >= LARGEUR-distance_joueur) {
	 	return 0;
	} else if (*l2 != NULL && (*l2)->val.y >= LARGEUR-distance_joueur) {
	 	return 0;
	} else if (*l3 != NULL && (*l3)->val.y >= LARGEUR-distance_joueur) {
	 	return 0;
	} else {
	      	return 1;
	}	
}



void initialisation_mechant(listeEntites* l1, listeEntites* l2, listeEntites* l3, int sprite1, int sprite2, int sprite3) {
	*l1 = NULL;
	*l2 = NULL;
	*l3 = NULL;
	int y_l1 = 50;
	
	creer_ligne_mechant(l1, y_l1, sprite1, 1);
	creer_ligne_mechant(l2, y_l1*2, sprite2, 0);
	creer_ligne_mechant(l3, y_l1*3, sprite3, 0);
}



void initialisation_joueur_var(entite* joueur, int canon, int* gagne, int* perdu, int* nb_vie, int* direction) {
	(*joueur).x = LONGUEUR / 2 - 40;
	(*joueur).y = LARGEUR - 100;
	(*joueur).sprite = canon;

	*gagne = 1;
	*perdu = 1;
	*nb_vie = 3;
	*direction = 1;
}



void initialisation_bombe_missile_bouclier(listeEntites* lBom, listeEntites* lM, listeEntites* lBou, int bouclier) {
	*lBom = NULL;
	*lM = NULL;
	*lBou = NULL;
	int hauteur_bouclier = 180;

	creer_ligne_bouclier(lBou, LARGEUR-hauteur_bouclier, bouclier);
}


/* Affichage */

void afficher_liste(listeEntites l) {
	while (l != NULL) {
		afficherLutin(l->val.sprite, l->val.x, l->val.y);
		l = l->next;
	}
}



void afficher_joueur (entite e, int sprite) {
	afficherLutin(sprite, e.x, e.y);
}



void afficher_vie (int nb_vie, int vies, int vie1, int vie2, int vie3) {
	int taille_texte1 = 100;
	int taille_texte2 = 80;
	afficherLutin(vies, LONGUEUR/2-taille_texte1, 0);
	if (nb_vie == 3) {
		afficherLutin(vie3, LONGUEUR/2+taille_texte2, 0);
	} else if (nb_vie == 2) {
		afficherLutin(vie2, LONGUEUR/2+taille_texte2, 0);
	} else {
		afficherLutin(vie1, LONGUEUR/2+taille_texte2, 0);
	}
}

/* Main */

int main() {
	// Creation de la fenetre
	char titre[25] = "Space_Invaders";
	creerSurface(LONGUEUR, LARGEUR, titre);


	// Chargement des lutins
	int monstre_1_1 = chargerLutin("Lutins/invader_monstre1_1.bmp", COULEUR_NOIR);
	int monstre_2_1 = chargerLutin("Lutins/invader_monstre2_1.bmp", COULEUR_NOIR);
	int monstre_3_1 = chargerLutin("Lutins/invader_monstre3_1.bmp", COULEUR_NOIR);	
	int canon = chargerLutin("Lutins/invader_canon.bmp", COULEUR_NOIR);
	int canon_mort = chargerLutin("Lutins/invader_canon_ferraille.bmp", COULEUR_NOIR);
	int bouclier = chargerLutin("Lutins/invader_bouclier.bmp", COULEUR_NOIR);
	int bombe = chargerLutin("Lutins/invader_bombe.bmp", COULEUR_NOIR);
	int missile = chargerLutin("Lutins/invader_missile.bmp", COULEUR_NOIR);
	int vies = lutinTexte("Nombre de vies :", COULEUR_BLANC);
	int vie1 = lutinTexte("1", COULEUR_BLANC);
	int vie2 = lutinTexte("2", COULEUR_BLANC);
	int vie3 = lutinTexte("3", COULEUR_BLANC);
	int victoire = lutinTexte("Vous avez gane", COULEUR_BLANC);
	int defaite = lutinTexte("Vous avez perdu", COULEUR_BLANC);
	int information = lutinTexte("Appuyez sur r pour rejouer", COULEUR_BLANC);


	
	// Initialisation
	listeEntites listeMechant1 = NULL;
	listeEntites listeMechant2 = NULL;
	listeEntites listeMechant3 = NULL;
	listeEntites listeBombe = NULL;
	listeEntites listeMissile = NULL;
	listeEntites listeBouclier = NULL;
	entite joueur;	
	
	
	evenement evt;
	char touche;
	int direction; // Permet de savoir dans quel sens se deplace les invaders
	int perdu; // Permet de savoir si le joueur est rattrapé par les invaders
	int gagne; // Permet de savoir si le joueur a tué tous les invaders
	int nb_vie; // Pemet de définir le nombre de vies du joueur
	int jouer = 1; // Permet de savoir si le jeu est fini ou quitter
	int delai = 0; // Permet de gerer le delai entre 2 missiles
	int pause = 1; // Permet de mettre le jeu en pause
		       //
	int x_texte1 = 120;
	int y_texte1 = 50;	
	int x_texte2 = 80;
	int y_texte2 = 100;

	initialisation_mechant(&listeMechant1, &listeMechant2, &listeMechant3, monstre_1_1, monstre_2_1, monstre_3_1);
	initialisation_bombe_missile_bouclier(&listeBombe, &listeMissile, &listeBouclier, bouclier);
	initialisation_joueur_var(&joueur, canon, &gagne, &perdu, &nb_vie, &direction);


	while (jouer) {
		rectanglePlein(0, 0, LONGUEUR, LARGEUR, COULEUR_NOIR);
		lireEvenement(&evt, &touche, NULL);
		jouer = (quitter != evt);	

		
		if (perdu && gagne && nb_vie > 0) {	
			// Cas ou le jeu n'est pas fini
			if (pause) {
				// Cas où le jeu n'est pas en pause
				
		       		mvt_invaders(&listeMechant1, &listeMechant2, &listeMechant3, &direction);
				mvt_invaders(&listeMechant2, &listeMechant1 ,&listeMechant3, &direction);
				mvt_invaders(&listeMechant3, &listeMechant1, &listeMechant2, &direction);

				deplacer_bombe(&listeBombe);
				deplacer_missile(&listeMissile);

				deplacer_joueur(evt, touche, &joueur);
				ajouter_bombe(listeMechant1, &listeBombe, bombe);
				ajouter_bombe(listeMechant2, &listeBombe, bombe);
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

			}


			afficher_liste(listeMechant1);
			afficher_liste(listeMechant2);
			afficher_liste(listeMechant3);
			afficher_liste(listeBombe);
			afficher_liste(listeMissile);
			afficher_liste(listeBouclier);	
			afficher_vie(nb_vie, vies, vie1, vie2, vie3);

			mettre_pause(evt, touche, &pause);

			if (perdu && nb_vie > 0) {
				afficher_joueur(joueur, canon);
			} else {
				afficher_joueur(joueur, canon_mort);
			}
		} else {
			// Cas où le jeu est fini
			afficherLutin(information, LONGUEUR/2-x_texte1, LARGEUR/2-y_texte1);		

			if (!gagne) {
				// Cas où le joueur a gagné
				afficherLutin(victoire, LONGUEUR/2-x_texte2, LARGEUR/2-y_texte2);		
			}
			if (!perdu || nb_vie <= 0) {
				// Cas où le joueur a perdu
				afficherLutin(defaite, LONGUEUR/2-x_texte2, LARGEUR/2-y_texte2);
			}
			if (touche == 'r') {
				// Cas où le joueur veut rejouer
				initialisation_mechant(&listeMechant1, &listeMechant2, &listeMechant3, monstre_1_1, monstre_2_1, monstre_3_1);
				initialisation_bombe_missile_bouclier(&listeBombe, &listeMissile, &listeBouclier, bouclier);
				initialisation_joueur_var(&joueur, canon, &gagne, &perdu, &nb_vie, &direction);
			}
		}


		
		SDL_Delay(15);
		majSurface();
	}

	

   

  	fermerSurface();
  	return 0;
}
