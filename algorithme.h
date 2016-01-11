#ifndef ALGO_H_INCLUDED
#define ALGO_H_INCLUDED
#include <stdbool.h>
#include "graphe.h"


#define PARCOURU 0
#define POIDS 1
#define ANTECEDENT 2
#define NEW_POIDS 3
#define ORDRE 0
#define DEGRE_ENTRANT 1

void plusCourtChemin(matrice_adjacente *m, int sommet, char nom[]);
void dijkstra(matrice_adjacente * m, int**, int sommet);
void initialisation_dijkstra(int **,int sommet, int nb_sommet);
int chemin_min(matrice_adjacente m, int sommet);
void update(int poidsPere, int, int sommet_pere, int sommet_fils, int **etat_dji);
void afficherCheminPlusCourt(int **etat_final, int sommet_depart, int sommet_final);
bool verificationBellmanFord(int **distance, matrice_adjacente * m);
void initialisation_bellmanFord(int sommet,int **, int nb_sommet);
void bellman_ford(matrice_adjacente * m, int **, int sommet);
bool presencePoidsNegatif(matrice_adjacente * m);
int * tri_topologique(matrice_adjacente * m);
void mettreAJourMatriceCalculTopo(int ** m, int sommet, int degre);
bool rotation(int **matrice_calcul_topo, int nombre_sommet, int * ordre_topologique, int *taille_topo, int *taille_topo_max);
void trouverSommet(int ** m, int degre, int * ordre_topo, int min, int max);
void bellman(matrice_adjacente * m, int **,int * ordre_topologique, int sommet);
int choixSommet(matrice_adjacente *m);
void initialisation_bellman(int **, int nb_sommet,int origine);
void afficheGrapheDotFinal(int nombre_sommet, int ** distance, matrice_adjacente * m, char nom[50]);

#endif // ALGO_H_INCLUDED
