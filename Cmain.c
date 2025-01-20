#include <stdio.h>
#include <stdlib.h>
#include "Caffichage.h"
#include "struct_etud.h"
#include <time.h>
#define TAILLE_MAX 100

int main(int argc, char* argv[])
{
    FILE* fichierVague = NULL;
    fichierVague = fopen("ex_vague.txt", "r");
    
    char cagnotte[TAILLE_MAX]= "";
 
    if (fichierVague != NULL)
    {
        //gestion cagnotte
        fgets(cagnotte, TAILLE_MAX, fichierVague);
        printf("Credit restant : %s\n", cagnotte);
        //afficher les vagues AVANT le debut du jeu
        afficheVagues(fichierVague);
        rewind(fichierVague);
        Liste *l=placer(fichierVague);
        liberer_liste(l);
        fclose(fichierVague);
    }
    else
    {
        printf("Erreur");
    }
    return 0;
}
