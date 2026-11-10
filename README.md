# Space Invaders

Reproduction du jeu d'arcade **Space Invaders** en **C**, avec la bibliothèque graphique **SDL 1.2**.

## Sommaire

- [Fonctionnalités](#fonctionnalités)
- [Compilation](#compilation)
- [Lancement](#lancement)
- [Commandes](#commandes)
- [Règles du jeu](#règles-du-jeu)
- [Structure du projet](#structure-du-projet)
- [Crédits](#crédits)

## Fonctionnalités

- Trois rangées d'envahisseurs (12, 11 et 11 ennemis) qui avancent latéralement et descendent à chaque bord atteint
- Bombes lâchées au hasard par les envahisseurs
- Missiles du joueur, avec un délai de recharge entre deux tirs
- Trois boucliers qui absorbent les tirs
- Compteur de vies affiché à l'écran
- Pause, écran de victoire / défaite et relance de la partie

## Compilation

```bash
make
```

Le `Makefile` compile d'abord la bibliothèque graphique (`Graphique/`), puis le jeu.

Autres cibles utiles :

| Commande       | Effet                                         |
|----------------|-----------------------------------------------|
| `make clean`   | Supprime les fichiers objets et l'exécutable  |
| `make rebuild` | Nettoie puis recompile tout                   |

## Lancement

Depuis la racine du projet (les sprites sont chargés avec des chemins relatifs) :

```bash
./space_invaders
```

## Commandes

| Touche | Action                                    |
|--------|-------------------------------------------|
| `q`    | Déplacer le joueur vers la gauche         |
| `d`    | Déplacer le joueur vers la droite         |
| `z`    | Tirer un missile                          |
| `p`    | Mettre / retirer la pause                 |
| `r`    | Relancer le jeu une fois la partie finie  |

Les touches correspondent à un clavier AZERTY. Pour quitter, il suffit de fermer la fenêtre.

## Règles du jeu

- Le joueur commence avec **3 vies**.
- **Victoire** : tous les envahisseurs sont détruits.
- **Défaite** : le joueur n'a plus de vie, un envahisseur le touche ou la rangée d'envahisseurs atteint le bas de l'écran.
- Les missiles détruisent les envahisseurs et peuvent annuler les bombes qu'ils croisent.
- Les boucliers disparaissent lorsqu'ils sont touchés par une bombe ou un missile.

## Structure du projet

```text
.
├── main.c               # Boucle de jeu, déplacements, collisions, affichage
├── Makefile             # Compilation du jeu (et de la bibliothèque graphique)
├── Graphique/           # Bibliothèque graphique au-dessus de SDL
│   ├── libgraph.c
│   ├── libgraph.h
│   └── Makefile
├── ListeChainee/        # Liste chaînée d'entités (position + sprite)
│   ├── liste.c
│   └── liste.h
└── Lutins/              # Sprites au format BMP
```

Les envahisseurs, les bombes, les missiles et les boucliers sont tous stockés dans des listes chaînées d'entités.
