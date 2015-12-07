#ifndef ALGO_H_INCLUDED
#define ALGO_H_INCLUDED
#include <stdbool.h>
#include "graphe.h"

void dijkstra(matrice_adjacente * m, int sommet);
int chemin_min(matrice_adjacente m, int sommet);
void update(int poidsPere, int sommet_pere, int sommet_fils, int **etat_dji);
void afficherCheminPlusCourt(int **etat_final, int sommet_depart, int sommet_final);
bool verificationBellmanFord(int **distance, matrice_adjacente * m);

#endif // ALGO_H_INCLUDED
