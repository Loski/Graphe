#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "algorithme.h"



void plusCourtChemin(matrice_adjacente *m, int sommet)
{
    if(!presencePoidsNegatif(m)){
            printf("\nOn utilise l'algorithme de Dijkstra !\n");
        dijkstra(m, sommet);
        return;
    }
    int * listeTopo = tri_topologique(m),i;
    if(NULL == listeTopo){
        printf("Présence d'un cycle. Testons Ford Bellman !");
        bellman_ford(m, sommet);
    }
    else{
        printf("\nOn utilise l'algorithme de Bellman !\n");
        bellman(m , listeTopo, sommet);
    }
}
int choixSommet(matrice_adjacente * m){
    int choixJ;
    do{
        printf("De quel sommet voulez vous regarder?\t0 - %d\n", m->nombre_sommet-1);
        scanf("%d", &choixJ);
        while(getchar()!='\n')
            continue;
    }while( choixJ < 0 || choixJ>m->nombre_sommet-1);
    printf("Vous avez choisis le sommet %d", choixJ);
    return choixJ;
}
bool presencePoidsNegatif(matrice_adjacente * m){
    int i,j;
    for(i = 0; i < m->nombre_sommet ; i++)
         for(j = 0; j < m->nombre_sommet ; j++)
            if(m->matrice[i][j][POIDS] < 0)
                return true;
    return false;
}

void afficheGraphe(matrice_adjacente m)
{
    int i,j;
    printf ("digraph mon_graphe {");
    for (i=0; i<m.nombre_sommet; i++)
    {
        for (j=0; j<m.nombre_sommet; j++)
        {
            if(m.matrice[i][j][PARCOURU]==1)
            {
                printf("%d->%d[LABEL=%d];",i,j,m.matrice[i][j][POIDS]);
            }
        }
    }
    printf("}");
}
void afficheGrapheDotFinal(int nombre_sommet, int ** distance, matrice_adjacente * m){
    int i;
    printf ("digraph mon_graphe {");
    for (i=0; i<nombre_sommet; i++)
    {
        if(distance[i][ANTECEDENT] != -1)
            printf("%d->%d[LABEL=%d];",distance[i][ANTECEDENT],i,m->matrice[distance[i][ANTECEDENT]][i][POIDS]);
    }
    printf("}");
}
void initialisation_dijkstra(int ** distance, int sommet, int nb_sommet)
{
    int i;
    for(i = 0; i < nb_sommet; i++)
    {
        distance[i] = malloc(sizeof(int) * 3);
        distance[i][PARCOURU] = false;
        distance[i][POIDS] = -1;
        distance[i][ANTECEDENT] = -1;
    }
    distance[sommet][POIDS] = 0;  // On initialise le sommet de départ à 0.
}
void dijkstra(matrice_adjacente * m, int sommet)
{
    int **distance = malloc(sizeof(int*) * m->nombre_sommet), i, j;
    initialisation_dijkstra(distance, sommet, m->nombre_sommet);
    for(i = 0; i < m->nombre_sommet; i++)  // pour cq sommet i
    {
        int poids_actu = -1, sommet_actu = -1;
        for(j = 0; j < m->nombre_sommet; j++) // pour cq sommet j
        {
            if(!distance[j][PARCOURU] && distance[j][POIDS] > -1 && (distance[j][POIDS] < poids_actu || sommet_actu == -1)){
                /*Si le sommet j n'a pas été parcouru, que son poids est sup à 0 et que le poids du sommet j est inférieur ou que le sommet actuel
                 est égal à -1, donc aucun sommet marqué. On marque ainsi le sommet non parcouru, ayant le plus petit poids positif */
                sommet_actu = j;
                poids_actu = distance[j][POIDS];
            }
        }
        if(sommet_actu != -1)
        {
            for(j = 0; j < m->nombre_sommet; j++)
            {
                if(m->matrice[sommet_actu][j][CHEMIN] == 1)  // sommet = -1
                {
                    // on met à jours chaque poids
                    update(distance[sommet_actu][POIDS], m->matrice[sommet_actu][j][POIDS], sommet, j, distance);
                }
            }
            distance[sommet_actu][PARCOURU] = true;
        }
    }
    for(i = 0 ; i < m->nombre_sommet; i++)
        afficherCheminPlusCourt(distance, sommet, i);
    afficheGrapheDotFinal(m->nombre_sommet, distance, m);
}



void update(int poidsPere, int poidsfils, int sommet_pere, int sommet_fils, int **etat_dji)
{
    if(!etat_dji[sommet_fils][PARCOURU] && (etat_dji[sommet_fils][POIDS] <= 0 || poidsfils + poidsPere < etat_dji[sommet_fils][POIDS]))
    {
        etat_dji[sommet_fils][POIDS] = poidsfils + poidsPere;
        etat_dji[sommet_fils][ANTECEDENT] = sommet_pere;
    }
    return;
}


void afficherCheminPlusCourt(int **etat_final, int sommet_depart, int sommet_final)
{
    if(sommet_depart == sommet_final)
        return;
    int current = sommet_final;
    int i = 0;
    int poids = 0;
    while(current != sommet_depart)
    {
        if(current == -1)
            return;
        poids += etat_final[current][POIDS];
        current = etat_final[current][ANTECEDENT];
    }
    printf("\nde %d au sommet %d : %d.\n", sommet_depart, sommet_final, poids);
}

int ** initialisation_bellmanFord(int sommet, int nb_sommet)
{
    int **distance = malloc(sizeof(int*) * nb_sommet), i;
    for(i = 0; i < nb_sommet; i++)
    {
        distance[i] = malloc(sizeof(int) * 2);
        distance[i][ANTECEDENT] = -1;
        distance[i][POIDS] = INT_MAX;
    }
    distance[sommet][POIDS] = 0;
    return distance;
}

void bellman_ford(matrice_adjacente * m, int sommet)
{
    int **distance = initialisation_bellmanFord(sommet, m->nombre_sommet), i, j, k;
    bool change = false;
    for(i  = 1; i < m->nombre_sommet; i++) // max nombre de sommet - 1 itérations
    {
        if(change)
            break;
        change = false;
        for(j = 0; j < m->nombre_sommet; j++)
        {
            for(k = 0; k < m->nombre_sommet; k++)
                if(m->matrice[j][k][CHEMIN] == 1 && distance[j][POIDS] != INT_MAX)
                {
                    int pds = m->matrice[j][k][POIDS];
                    if(distance[k][POIDS]  == INT_MAX || distance[j][POIDS] + pds < distance[k][POIDS])
                    {
                        distance[k][POIDS] = distance[j][POIDS]  + pds;
                        distance[k][ANTECEDENT] = j;
                        change = true;    // Détecte un changement
                    }
                }
        }
    }
    if(change == true && verificationBellmanFord(distance, m)){
        for(i = 0; i < m->nombre_sommet; i++)
            if(sommet != i)
                printf("\nde %d au sommet %d : %d.\n", sommet, i, distance[i][POIDS]);
        afficheGrapheDotFinal(m->nombre_sommet, distance, m);
    }
    else
        printf("Présence d'un circuit absorbant. Impossible de trouver un chemin minimum.");
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

int * tri_topologique(matrice_adjacente *m){
    int i, j;
    int ** matrice_calcul_topo = malloc(sizeof(int)* m->nombre_sommet), *ordre_topologique = malloc(sizeof(int)* m->nombre_sommet), taille_topo = 0, taille_topo_max = 0;

    for(i = 0 ; i < m->nombre_sommet; i++){
        matrice_calcul_topo[i] = calloc(sizeof(int), m->nombre_sommet +1);
        for(j = 0; j < m->nombre_sommet; j++){
            matrice_calcul_topo[i][j] = m->matrice[i][j][CHEMIN];
        }
    }

    int compteur = 0;
    while(compteur != m->nombre_sommet){ // Max d'itération = le nombre de sommet
        if(taille_topo_max == m->nombre_sommet){ //Même nombre de sommets donc on a tous trié.
            return ordre_topologique;
        }
        for(i = 0 ; i < m->nombre_sommet; i++)
        {
            if(matrice_calcul_topo[i][m->nombre_sommet] > 0)
                matrice_calcul_topo[i][m->nombre_sommet] = 0; // on met les degrés entrants des sommets non marqués à 0
            for(j = 0; j < m->nombre_sommet; j++)
            {
                matrice_calcul_topo[i][m->nombre_sommet] += matrice_calcul_topo[j][i]; // degrés entrant du sommet i.
            }
        }
        if(!rotation(matrice_calcul_topo, m->nombre_sommet, ordre_topologique, &taille_topo, &taille_topo_max)) // pas de rotation = boucle
            return NULL;
        compteur++;
    }

    return ordre_topologique;
}
bool rotation(int ** m, int degre, int *ordre_topologique, int * min, int * max){
    int i;
    for(i = 0 ; i < degre; i++)
    {
        if(m[i][degre] == 0){
            ordre_topologique[*max] = i; // on ajouter le sommet
            (*max)++;
        }
    }
    if(*min == *max) // Pas de sommets avec un degrés entrants de 0 => boucle.
        return false;
    trouverSommet(m, degre, ordre_topologique, *min, *max);
    *min = *max;
    return true;
}
void trouverSommet(int ** m, int degre, int * ordre_topo, int min, int max){
    int i;
    for(i = min; i < max; i++)
        mettreAJourMatriceCalculTopo(m, ordre_topo[i], degre);
}


// on supprime les arcs sortants du sommet.
void mettreAJourMatriceCalculTopo(int ** m, int sommet, int degre){
    int i;
    for(i = 0; i < degre; i++){
        m[sommet][i] = 0;
    }
    m[sommet][degre] = -1;
}


int ** initialisation_bellman(int sommet, int nb_sommet, int origine)
{
    int **distance = malloc(sizeof(int*) * nb_sommet), i;
    for(i = 0; i < nb_sommet; i++)
    {
        distance[i] = malloc(sizeof(int) * 2);
        distance[i][POIDS] = INT_MAX;
        distance[i][ANTECEDENT] = -1;
    }
    distance[origine][POIDS] = 0;
    return distance;
}

void bellman(matrice_adjacente * m, int * ordre_topologique, int sommet)
{
    printf("int max = %d", INT_MAX);
    int ** distance = initialisation_bellman(sommet, m->nombre_sommet, ordre_topologique[0]), i, j;
    for(i = 0; i < m->nombre_sommet; i++)
    {
        for(j = 0; j < m->nombre_sommet; j++)
            if(m->matrice[ordre_topologique[i]][j][CHEMIN] == 1 && distance[ordre_topologique[i]][POIDS] != INT_MAX && ordre_topologique[i] != j)  // Si il y a un chemin entre le premier sommet classé et j
            {
                int pds = distance[ordre_topologique[i]][POIDS] + m->matrice[ordre_topologique[i]][j][POIDS];
                if(distance[j][POIDS]  == INT_MAX ||  pds < distance[j][POIDS])
                {
                    printf("DIS TANCE %d\n", distance[j][POIDS]);
                    distance[j][POIDS] = pds;
                    distance[j][ANTECEDENT] = j;
                }
            }
    }
    for(i = 0; i < m->nombre_sommet; i++)
        if(sommet != i)
            printf("\nde %d au sommet %d : %d.\n", sommet, i, distance[i][POIDS]);
    afficheGrapheDotFinal(m->nombre_sommet, distance, m);
}


