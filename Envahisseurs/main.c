#include "Graphique/libgraph.h"
#include <SDL/SDL.h>

int main() {
  char titre[25] = "Space_Invaders";
  creerSurface(1000, 1000, titre);

  int lutin1 = chargerLutin("Lutins/invader_monstre2_2.bmp", 0);
  afficherLutin(lutin1, 100, 100);
  majSurface();
  SDL_Delay(1000);

  rectanglePlein(0, 0, 1000, 1000, 255);
  majSurface();
  SDL_Delay(1000);

	
  afficherLutin(lutin1, 500, 500);
  majSurface();
  SDL_Delay(1000);

  fermerSurface();
  return 0;
}
