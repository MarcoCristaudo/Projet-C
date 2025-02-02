#include <stdio.h>
#include <stdlib.h>
#include "Caffichage.h"
#include "struct_etud.h"
#include "struct_jeu.h"
#include "Tourelle.h"
#include <time.h>
#include <string.h>

#define TAILLE_MAX 100
//#define Fichier_vague ... a faire quand on aura plusieurs mode de jeu pour bonne gestion memoire en cas d 'erreur
int main(int argc, char* argv[])
{
    afficher_meilleurs_scores();
    FILE* fichierVague = NULL;
    //choix du niveau et ouverture fichier correspondant
    int choixNiveau = 0;
    printf("Choisissez le niveau entre 1 et 5 (1 étant facile et 5 difficile) : \n");
    scanf("%d", &choixNiveau);
    switch(choixNiveau)
    {
        case 1 :
            fichierVague = fopen("lvl1.txt", "r");
            break;
        case 2 :
            fichierVague = fopen("lvl2.txt", "r");
            break;
        case 3 :
            fichierVague = fopen("lvl3.txt", "r");
            break;
        case 4 :
            fichierVague = fopen("lvl4.txt", "r");
            break;
        case 5 :
            fichierVague = fopen("lvl5.txt", "r");
            break;
        default :
            printf("Choix du niveau invalide.\n");
            exit(1);
            break;
    }
    
    char cagnotte[TAILLE_MAX]= "";
 
    if ((fichierVague != NULL) && (fichierConforme(fichierVague)))
    {
        //gestion cagnotte
        fgets(cagnotte, TAILLE_MAX, fichierVague);
        printf("Credit restant : %s\n", cagnotte);
        
        //afficher les vagues AVANT le debut du jeu
        afficheVagues(fichierVague);
        rewind(fichierVague);
        /*on initialise jeu*/
        Jeu* jeu=malloc(sizeof(Jeu));
        jeu->tour=0;
        jeu->cagnotte=atoi(cagnotte);
        jeu->tourelles=NULL;
        jeu->derniere=NULL;
        jeu->etudiants=NULL;
        jeu->dernier=NULL;
        //on chaine les étudiants
        placer(jeu,fichierVague);
        fclose(fichierVague);
        //on chaine les étudiants par rapport a leurs lignes
        connecte_ligne(jeu);
        //on chaine les tourelles
        chainage_tourelle(jeu);
        //on lance le jeu
        affichejeu(jeu);
    }
    else
    {
        fclose(fichierVague);
        printf("Erreur.\n");
    }
    return 0;
}