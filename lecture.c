#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"

int chargementGraphe(graphe_chargement *g)
{
    FILE * fichier = NULL;
    int i;
    fichier = fopen("test.txt","rt");
    if(fichier == NULL)
    {
        printf("\nImpossible d'ouvrir la sauvegarde\n");
        exit(EXIT_FAILURE);
    }
    fscanf(fichier, "%d %d", &(g->nombre_sommet), &(g->nombre_arc));
    g->arc_graphe = malloc(sizeof(arc) * g->nombre_arc);
    if( g->arc_graphe == NULL )
    {
        fprintf(stderr,"Allocation impossible");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < g->nombre_arc; i++)
    {
        fscanf(fichier, "%d,%d,%d", &(g->arc_graphe[i].head), &(g->arc_graphe[i].queue), &(g->arc_graphe[i].poids));
    }
    return 1;
}

void transformGraphe(graphe_chargement g, matrice_adjacente *m)
{
    int i,j;
    m->nombre_sommet = g.nombre_sommet;
    m->matrice = (int***)malloc(g.nombre_sommet* sizeof(int**));
    for(i = 0; i < g.nombre_sommet; i++)
    {
        m->matrice[i] = (int**)malloc(g.nombre_sommet* sizeof(int*));
        for(j = 0; j < g.nombre_sommet; j++)
        {
            m->matrice[i][j] = (int*) calloc(2, sizeof(int));
        }
    }
    for(i = 0; i < g.nombre_arc; i++)
    {
        m->matrice[g.arc_graphe[i].head-1][g.arc_graphe[i].queue-1][CHEMIN] = 1;
        m->matrice[g.arc_graphe[i].head-1][g.arc_graphe[i].queue-1][POIDS] =  g.arc_graphe[i].poids;
    }

    afficherGraphe(*m, CHEMIN);
    afficherGraphe(*m, POIDS);
}


// Type _> Soit la constante poids, soit chemin
void afficherGraphe(matrice_adjacente m, int type)
{
    int i,j;
    printf("\n");
    for(i =0; i < m.nombre_sommet; i++)
    {
        for(j =0; j < m.nombre_sommet; j++)
            printf("%d\t", m.matrice[i][j][type]);
        printf("\n");
    }
}


