#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define PARCOURU 0
#define ANTECEDENT 2
#define NEW_POIDS 3
#include <math.h>
#include "algorithme.h"

int ** initialisation_dijkstra(int sommet, int nb_sommet){
    int **distance = malloc(sizeof(int*) * nb_sommet), i;
    for(i = 0; i < nb_sommet; i++){
        distance[i] = malloc(sizeof(int) * 3);
        distance[i][PARCOURU] = false;
        distance[i][POIDS] = -1;
        distance[i][ANTECEDENT] = -1;
    }
    distance[sommet][POIDS] = 0;
    return distance;
}
int choixAlgo(matrice_adjacente *m){
}
void afficheGraphe(matrice_adjacente m)
{
    int i,j;
    printf ("digraph mon_graphe {");
    for (i=0;i<m.nombre_sommet;i++)
    {
        for (j=0;j<m.nombre_sommet;j++)
        {
            if(m.matrice[i][j][PARCOURU]==1)
            {
                printf("%d->%d[LABEL=%d]",i,j,m.matrice[i][j][POIDS]);
            }
        }
    }
    printf("}");
}
void dijkstra(matrice_adjacente * m, int sommet){
    int **distance = initialisation_dijkstra(sommet, m->nombre_sommet), i, j;
    for(i = 0; i < m->nombre_sommet; i++){
        int poids_actu = -1, sommet_actu = -1;
        for(j = 0; j < m->nombre_sommet; j++){
            if(!distance[j][PARCOURU] && distance[j][POIDS] > -1 && (distance[j][POIDS] < poids_actu || sommet_actu == -1))
                sommet_actu = j;
                poids_actu = distance[j][POIDS];
        }
        if(sommet_actu != -1){
        for(j = 0; j < m->nombre_sommet; j++){
            if(m->matrice[sommet_actu][j][CHEMIN] == 1){ // sommet = -1
                update(distance[sommet_actu][POIDS], sommet, j, distance);
            }
        }
        distance[sommet][PARCOURU] = true;
        }
    }
    afficherCheminPlusCourt(distance, sommet, 1);
}


int chemin_min(matrice_adjacente m, int sommet){
    int i, sommetPlusProche, distance = -1;
    for(i = 0; i < m.nombre_sommet; i++){
        if(m.matrice[sommet][i][CHEMIN] == 1)
            if(m.matrice[sommet][i][POIDS] < distance || distance < 0){
                distance = m.matrice[sommet][i][POIDS];
                sommetPlusProche = m.matrice[sommet][i][CHEMIN];
            }
    }
    return sommetPlusProche;
}

void update(int poidsPere, int sommet_pere, int sommet_fils, int **etat_dji){
    if(!etat_dji[sommet_fils][PARCOURU] && (etat_dji[sommet_fils][POIDS] <= 0 || etat_dji[sommet_pere][POIDS] + poidsPere < etat_dji[sommet_fils][POIDS])){
        etat_dji[sommet_fils][POIDS] = etat_dji[sommet_pere][POIDS] + poidsPere;
        etat_dji[sommet_fils][ANTECEDENT] = sommet_pere;
    }
    return;
}

int selectionPoidsPlusFaible(int **etat_dij, int taille)
{
    int i, sommetMin = -1;
    for(i = 0; i < taille; i++)
    {
        if(!etat_dij[i][PARCOURU])
        {
            if(etat_dij[i][POIDS] < sommetMin || sommetMin < 0)
                sommetMin = etat_dij[i][POIDS];
        }
    }
    return sommetMin;
}

void afficherCheminPlusCourt(int **etat_final, int sommet_depart, int sommet_final){
    if(sommet_depart == sommet_final)
        return;
    int current = sommet_final;
    printf(" Sommet %d", current);
    while(current != sommet_depart){
        if(current == -1)
            return;
        current = etat_final[current][ANTECEDENT];
        printf(" Sommmet %d", current);
    }
}

int ** initialisation_bellman(int sommet, int nb_sommet){
    int **distance = malloc(sizeof(int*) * nb_sommet), i;
    for(i = 0; i < nb_sommet; i++){
        distance[i] = malloc(sizeof(int) * 3);
        distance[i][PARCOURU] = false;
        distance[i][POIDS] = -1;
        distance[i][ANTECEDENT] = INFINITY;
        distance[i][NEW_POIDS] = -1;
    }
    distance[sommet][POIDS] = 0;
    distance[sommet][NEW_POIDS] = 0;
    return distance;
}

void belman_ford(matrice_adjacente * m, int sommet)
{
    int **distance = initialisation_bellman(sommet, m->nombre_sommet), i, j, k;
    for(i  = 1; i < m->nombre_sommet ; i++)
    {
        for(j = 0; j < m->nombre_sommet; j++)
        {
            for(k = 0; k < m->nombre_sommet; k++)
                if(m->matrice[j][k][CHEMIN] == 1)
                {
                    int pds = m->matrice[j][k][POIDS];
                    if(distance[j][POIDS]  + pds < distance[k][POIDS]){
                        distance[k][POIDS] = distance[j][POIDS]  + pds;
                    distance[k][ANTECEDENT] = j;
                    }
                }
        }
    }
    afficherCheminPlusCourt(distance, sommet, 2);
    if(verificationBellmanFord(distance, m))
        printf("hello");
    else
        printf("Présence d'un circuit absorbant.");
}

bool verificationBellmanFord(int **distance, matrice_adjacente * m)
{
    int j, k;
    for(j = 0; j < m->nombre_sommet; j++)
    {
        for(k = 0; k < m->nombre_sommet; k++)
            if(m->matrice[j][k][CHEMIN] == 1)
            {
                int pds = m->matrice[j][k][POIDS];
                if(distance[j][POIDS]  + pds < distance[k][POIDS])
                    return false;
                }
    }
    return true;
}



