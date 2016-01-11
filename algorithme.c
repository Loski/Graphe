#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "algorithme.h"

int nombre_operation;

void plusCourtChemin(matrice_adjacente *m, int sommet, char nom[50])
{
    int i,j;
    nombre_operation = 1;
    int **distance = malloc(sizeof(int*) * m->nombre_sommet);
    for(i = 0; i < m->nombre_sommet; i++){
        distance[i] = malloc(sizeof(int) * 3);
    }
    if(!presencePoidsNegatif(m)){
            printf("\nOn utilise l'algorithme de Dijkstra !\n");
        dijkstra(m, distance, sommet);
        return;
    }
    int * listeTopo = tri_topologique(m);
    nombre_operation += 2;
    if(NULL == listeTopo){
        printf("Présence d'un cycle. Testons Ford Bellman !");
        bellman_ford(m, distance, sommet);
    }
    else{
        printf("\nOn utilise l'algorithme de Bellman !\n");
        bellman(m , distance, listeTopo, sommet);
    }
        afficheGrapheDotFinal(m->nombre_sommet, distance, m, nom);
    printf("L'execution du programme pour votre graphe a nécessité %d opérations.", nombre_operation);

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
    nombre_operation+=2;
    for(i = 0; i < m->nombre_sommet ; i++)
         for(j = 0; j < m->nombre_sommet ; j++){
            nombre_operation+=2;
            if(m->matrice[i][j][POIDS] < 0)
                return true;
         }
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
void afficheGrapheDotFinal(int nombre_sommet, int ** distance, matrice_adjacente * m, char nom[50]){
    int i;
    char * ptr;
    FILE* fichier = NULL;
   if((ptr = strchr(nom , '.'))!= NULL)
        *ptr = '\0';
    fichier = fopen(strcat(nom, ".dot"), "w+");
    if(fichier == NULL)
        exit(EXIT_FAILURE);
    fprintf (fichier, "digraph mon_graphe {");
    for (i=0; i<nombre_sommet; i++)
    {
        if(distance[i][ANTECEDENT] != -1)
            fprintf(fichier, "%d->%d[LABEL=%d];",distance[i][ANTECEDENT],i,m->matrice[distance[i][ANTECEDENT]][i][POIDS]);
    }
    fprintf(fichier ,"}");
    fclose(fichier);
}
void initialisation_dijkstra(int ** distance, int sommet, int nb_sommet)
{
    int i;
    for(i = 0; i < nb_sommet; i++)
    {
        distance[i][PARCOURU] = false;
        distance[i][POIDS] = -1;
        distance[i][ANTECEDENT] = -1;
        nombre_operation += 4;
    }
    distance[sommet][POIDS] = 0;// On initialise le sommet de départ à 0.
    nombre_operation++;
}
void dijkstra(matrice_adjacente * m,int ** distance, int sommet)
{
    int i, j;
    nombre_operation++;
    initialisation_dijkstra(distance, sommet, m->nombre_sommet);
    for(i = 0; i < m->nombre_sommet; i++)  // pour cq sommet i
    {
        int poids_actu = -1, sommet_actu = -1;
        nombre_operation+= 2;
        for(j = 0; j < m->nombre_sommet; j++) // pour cq sommet j
        {
            if(!distance[j][PARCOURU] && distance[j][POIDS] > -1 && (distance[j][POIDS] < poids_actu || sommet_actu == -1)){
                /*Si le sommet j n'a pas été parcouru, que son poids est sup à 0 et que le poids du sommet j est inférieur ou que le sommet actuel
                 est égal à -1, donc aucun sommet marqué. On marque ainsi le sommet non parcouru, ayant le plus petit poids positif */
                sommet_actu = j;
                poids_actu = distance[j][POIDS];
                nombre_operation+=2;
            }
            nombre_operation+=4;
        }
        nombre_operation++;
        if(sommet_actu != -1)
        {
            for(j = 0; j < m->nombre_sommet; j++)
            {
                nombre_operation++;
                if(m->matrice[sommet_actu][j][CHEMIN] == 1)  // sommet = -1
                {
                    // on met à jours chaque poids
                    update(distance[sommet_actu][POIDS], m->matrice[sommet_actu][j][POIDS], sommet, j, distance);
                    nombre_operation++;
                }
            }
            distance[sommet_actu][PARCOURU] = true;
            nombre_operation++;
        }
    }
    for(i = 0 ; i < m->nombre_sommet; i++)
        afficherCheminPlusCourt(distance, sommet, i);
}



void update(int poidsPere, int poidsfils, int sommet_pere, int sommet_fils, int **etat_dji)
{
    nombre_operation+=3;
    if(!etat_dji[sommet_fils][PARCOURU] && (etat_dji[sommet_fils][POIDS] <= 0 || poidsfils + poidsPere < etat_dji[sommet_fils][POIDS]))
    {
        etat_dji[sommet_fils][POIDS] = poidsfils + poidsPere;
        etat_dji[sommet_fils][ANTECEDENT] = sommet_pere;
        nombre_operation+=2;
    }
    return;
}


void afficherCheminPlusCourt(int **etat_final, int sommet_depart, int sommet_final)
{
    if(sommet_depart == sommet_final)
        return;
    printf("Les plus courts chemins à partir de %d ont les valeurs suivantes", sommet_final);
    int current = sommet_final;
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

void initialisation_bellmanFord(int sommet, int ** distance, int nb_sommet)
{
    int i;
    nombre_operation++;
    for(i = 0; i < nb_sommet; i++)
    {
        distance[i][ANTECEDENT] = -1;
        distance[i][POIDS] = INT_MAX;
        nombre_operation += 3;
    }
    distance[sommet][POIDS] = 0;
    nombre_operation++;
    return;
}

void bellman_ford(matrice_adjacente * m, int ** distance, int sommet)
{
    initialisation_bellmanFord(sommet, distance, m->nombre_sommet);
    int i, j, k;
    bool change = false;
    nombre_operation+=2;
    for(i  = 1; i < m->nombre_sommet; i++) // max nombre de sommet - 1 itérations
    {
        nombre_operation+=2;
        if(change)
            break;
        change = false;
        for(j = 0; j < m->nombre_sommet; j++)
        {
            for(k = 0; k < m->nombre_sommet; k++)
                if(m->matrice[j][k][CHEMIN] == 1 && distance[j][POIDS] != INT_MAX)
                {
                    nombre_operation += 5;
                    int pds = m->matrice[j][k][POIDS];
                    if(distance[k][POIDS]  == INT_MAX || distance[j][POIDS] + pds < distance[k][POIDS])
                    {
                        distance[k][POIDS] = distance[j][POIDS]  + pds;
                        distance[k][ANTECEDENT] = j;
                        change = true;    // Détecte un changement
                        nombre_operation +=3;
                    }
                }
        }
    }
    if(verificationBellmanFord(distance, m)){
        printf("Les plus courts chemins à partir de %d ont les valeurs suivantes", sommet);
        for(i = 0; i < m->nombre_sommet; i++)
            if(sommet != i)
                printf("\nde %d au sommet %d : %d.\n", sommet, i, distance[i][POIDS]);
    }
    else
        printf("Présence d'un circuit absorbant. Impossible de trouver un chemin minimum.");
}

bool verificationBellmanFord(int **distance, matrice_adjacente * m)
{
    int j, k;
    for(j = 0; j < m->nombre_sommet; j++)
    {
        for(k = 0; k < m->nombre_sommet; k++){
            nombre_operation++;
            if(m->matrice[j][k][CHEMIN] == 1)
            {
                nombre_operation +=2;
                int pds = m->matrice[j][k][POIDS];
                if(distance[j][POIDS]  + pds < distance[k][POIDS])
                    return false;
            }
        }
    }
    return true;
}

int * tri_topologique(matrice_adjacente *m){
    int i, j;
    int **matrice_calcul_topo = malloc(sizeof(int)* m->nombre_sommet), *ordre_topologique = malloc(sizeof(int)* m->nombre_sommet), taille_topo = 0, taille_topo_max = 0;
    nombre_operation++;
    for(i = 0 ; i < m->nombre_sommet; i++){
        matrice_calcul_topo[i] = calloc(sizeof(int), m->nombre_sommet +1);
        nombre_operation++;
        for(j = 0; j < m->nombre_sommet; j++){
            matrice_calcul_topo[i][j] = m->matrice[i][j][CHEMIN];
            nombre_operation++;
        }
    }

    int compteur = 0;
    while(compteur != m->nombre_sommet){ // Max d'itération = le nombre de sommet
        nombre_operation++;
        if(taille_topo_max == m->nombre_sommet){ //Même nombre de sommets donc on a tous trié.
            return ordre_topologique;
        }
        for(i = 0 ; i < m->nombre_sommet; i++)
        {
            nombre_operation++;
            if(matrice_calcul_topo[i][m->nombre_sommet] > 0)
                matrice_calcul_topo[i][m->nombre_sommet] = 0; // on met les degrés entrants des sommets non marqués à 0
            for(j = 0; j < m->nombre_sommet; j++)
            {
                matrice_calcul_topo[i][m->nombre_sommet] += matrice_calcul_topo[j][i]; // degrés entrant du sommet i.
                nombre_operation++;
            }
        }
        nombre_operation++;
        if(!rotation(matrice_calcul_topo, m->nombre_sommet, ordre_topologique, &taille_topo, &taille_topo_max)) // pas de rotation = boucle
            return NULL;
        compteur++;
        nombre_operation++;
    }

    return ordre_topologique;
}
bool rotation(int ** m, int degre, int *ordre_topologique, int * min, int * max){
    int i;
    for(i = 0 ; i < degre; i++)
    {
        if(m[i][degre] == 0){
            nombre_operation+= 2;
            ordre_topologique[*max] = i; // on ajouter le sommet
            (*max)++;
        }
    }
    nombre_operation++;
    if(*min == *max) // Pas de sommets avec un degrés entrants de 0 => boucle.
        return false;
    trouverSommet(m, degre, ordre_topologique, *min, *max);
    *min = *max;
    nombre_operation+=2;
    return true;
}
void trouverSommet(int ** m, int degre, int * ordre_topo, int min, int max){
    int i;
    for(i = min; i < max; i++){
        mettreAJourMatriceCalculTopo(m, ordre_topo[i], degre);
        nombre_operation++;
    }
}


// on supprime les arcs sortants du sommet.
void mettreAJourMatriceCalculTopo(int ** m, int sommet, int degre){
    int i;
    for(i = 0; i < degre; i++){
        m[sommet][i] = 0;
        nombre_operation++;
    }
    m[sommet][degre] = -1;
    nombre_operation++;
}


void initialisation_bellman(int ** distance, int nb_sommet, int origine)
{
    int  i;
    for(i = 0; i < nb_sommet; i++)
    {
        distance[i][POIDS] = INT_MAX;
        distance[i][ANTECEDENT] = -1;
        nombre_operation+= 3;
    }
    distance[origine][POIDS] = 0;
    nombre_operation+= 2;
    return;
}

void bellman(matrice_adjacente * m, int ** distance, int * ordre_topologique, int sommet)
{
    initialisation_bellman(distance, m->nombre_sommet,ordre_topologique[0]);
    int i, j;
    nombre_operation++;
    for(i = 0; i < m->nombre_sommet; i++)
    {
        for(j = 0; j < m->nombre_sommet; j++){
            nombre_operation+= 3;
            if(m->matrice[ordre_topologique[i]][j][CHEMIN] == 1 && distance[ordre_topologique[i]][POIDS] != INT_MAX && ordre_topologique[i] != j)  // Si il y a un chemin entre le premier sommet classé et j
            {
                int pds = distance[ordre_topologique[i]][POIDS] + m->matrice[ordre_topologique[i]][j][POIDS];
                nombre_operation += 3;
                if(distance[j][POIDS]  == INT_MAX ||  pds < distance[j][POIDS])
                {
                    distance[j][POIDS] = pds;
                    distance[j][ANTECEDENT] = j;
                    nombre_operation += 2;
                }
            }
        }
    }
    printf("Les plus courts chemins à partir de %d ont les valeurs suivantes", sommet);
    for(i = 0; i < m->nombre_sommet; i++)
        if(sommet != i)
            printf("\nde %d au sommet %d : %d.\n", sommet, i, distance[i][POIDS]);
}


