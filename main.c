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
    dijkstra(&m, 2);
    belman_ford(&m, 2);
    return 0;
}

