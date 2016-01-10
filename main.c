#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"
#include "lecture.h"
#include "algorithme.h"

int main(int argc, char *argv[])
{
    graphe_chargement g;
    chargementGraphe(&g);
    matrice_adjacente m;
    transformGraphe(g, &m);
    //afficherGraphe(m,0);
    afficheGrapheMatrice(m, POIDS);
    int sommet = choixSommet(&m);
    plusCourtChemin(&m, sommet);
    return 0;
}

