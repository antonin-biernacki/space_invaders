#include <stdlib.h>

typedef struct entite {
	int x;
	int y;
	int sprite;
} entite;


typedef struct ll {
    entite val;
    struct ll *next;
} Node, *listeEntites;



listeEntites makeList(entite i, listeEntites next);


void addHead(entite i, listeEntites* l);


void addAfter(entite i, listeEntites l);


void deleteHead(listeEntites* l);

void deleteAfter(listeEntites l);


void freeList(listeEntites l);
