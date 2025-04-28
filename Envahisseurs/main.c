#include "Graphique/libgraph.h"
#include <unistd.h>

int main() {
  char titre[25] = "Space_Invaders";
  creerSurface(100, 100, titre);
  sleep(10);
  fermerSurface();
  return 0;
}
