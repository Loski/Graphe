#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"
#include "lecture.h"
#include "algorithme.h"

int main(int argc, char *argv[])
{
    char nom[50];
    graphe_chargement g;
    chargementGraphe(&g, nom);
    matrice_adjacente m;
    transformGraphe(g, &m);
    afficheGraphe(m , nom);
    int sommet = choixSommet(&m);
    plusCourtChemin(&m, sommet, nom);
    return 0;
}

