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



listeEntites makeList(entite i, listeEntites next) {
    listeEntites p=malloc(sizeof(Node));;
    p->val=i;
    p->next=next;
    return p;
}



void addHead(entite i, listeEntites* l) {
    listeEntites head = makeList(i, *l);
    *l=head;
}



void addAfter(entite i, listeEntites l) {
    listeEntites next = makeList(i, l->next);
    l->next = next;
}



void deleteHead(listeEntites* l) {
    listeEntites p=*l;
    *l=(*l)->next;
    free(p);
}



void freeList(listeEntites l) {
    while (l != NULL) {
        deleteHead(&l);
    }
}
