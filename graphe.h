#ifndef GRAPHE_H_INCLUDED
#define GRAPHE_H_INCLUDED


#define CHEMIN 0
#define POIDS 1


typedef struct arc
{
    int head;
    int queue;
    int poids;

} arc;

typedef struct graphe_chargement
{
    int nombre_arc;
    int nombre_sommet;
    arc *arc_graphe;
} graphe_chargement;


typedef struct matrice_adjacente
{
    int ***matrice;
    int nombre_sommet;
} matrice_adjacente;

void afficherGraphe(matrice_adjacente m, int type);


#endif // GRAPHE_H_INCLUDED
