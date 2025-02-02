#include <stdio.h>
#include <stdlib.h>
#include "Caffichage.h"
#include "struct_etud.h"
#include "struct_jeu.h"
#include "Tourelle.h"
#include <unistd.h>
#include <string.h>
#include <raylib.h>


#define TAILLE_CASE 100  //taille des cases de la grille
#define Y 150 //on descend de 90 pixel sur l'axe X
#define X 80  //on avance de 90 pixel sur l'axe 


//fontion qui affiche un terrain neutre
void afficheTerrainNeutre()
{
    for (int tour = 1; tour <= 9; tour++)
    {
        printf("Vague %d\n", tour);
        for (int i = 1; i <= 7; i++)
        {
            printf("%d|\t", i);
            for (int j = 1; j <= 15; j++)
            {
                printf(".\t");
            }
        printf("\n");
        }
    }
}

//fonction qui affiche les vagues AVANT le debut du jeu
void afficheVagues(FILE* fichierVague)
{
    //remplir le tableau de points 
    char affichage[NOMBRE_LIGNES][NOMBRE_TOURS]; //7 lignes et 9 colonnes
    for (int i = 0; i < NOMBRE_LIGNES; i++)
    {
        for (int j = 0; j < NOMBRE_TOURS; j++)
            affichage[i][j] = '.';
    }
    
    int numeroTour = 0;
    int numeroLigne = 0;
    char typeFichier = 0; 
    char chaine[100] = "";

    //transforme le fichier en l'affichage souhaite
    do
    {
    fscanf(fichierVague, "%d %d %c", &numeroTour, &numeroLigne, &typeFichier);
    affichage[numeroLigne-1][numeroTour-1] = typeFichier;
    } while (fgets(chaine, 100, fichierVague) != NULL);
    
    //afficher le tableau final
    for (int i = 0; i < NOMBRE_LIGNES; i++)
    {
        printf("%d|", i+1);
        for (int j = 0; j < NOMBRE_TOURS; j++)
            printf("%c\t", affichage[i][j]);
        printf("\n");
    }

}


void affichejeu(Jeu *jeu) {
    jeu->score=jeu->cagnotte;//plus on à dépensé d'argent, moins notre score au début est bon
    setenv("XMODIFIERS", "@im=none", 1);// on désactive l'IME (un systeme qui nous permet d'écrire des caractères en chinois,japonais etc qui nous sont inutile ici) pour éviter certaines fuites mémoire de raylib
    InitWindow(1700, 1200, "nom_jeu"); // Initialise la fenêtre
    SetTargetFPS(10); // Réduit la vitesse du jeu

    while (!WindowShouldClose() && jeu->nombre_etudiant != 0) {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        
        // Dessiner la grille avec un fond plus visible
        for (int i = 0; i < NOMBRE_LIGNES; i++) {
            for (int j = 0; j < 15; j++) {
            //on alterne la couleur des cases
            Color color = ((i + j) % 2 == 0) ? GREEN : DARKGREEN;
            DrawRectangle(X + j * TAILLE_CASE, Y + i * TAILLE_CASE, TAILLE_CASE, TAILLE_CASE, color);
            DrawRectangleLines(X + j * TAILLE_CASE, Y + i * TAILLE_CASE, TAILLE_CASE, TAILLE_CASE, GRAY);
            }
        }

        //on affiche les tourelles
        Tourelle *t = jeu->tourelles;
        while (t) {
            DrawText(TextFormat("  %d\n  %s\n" ,t->pointsDeVie,t->affichage), X + t->position * TAILLE_CASE + 10, Y + (t->ligne - 1) * TAILLE_CASE + 10, 20, WHITE);
            t = t->next;
        }
        
        // Afficher les étudiants avec des couleurs plus distinctes et des icônes
        Etudiant *e = jeu->etudiants;
        while (e && e->tour<=jeu->tour && e->position<15) {
            DrawText(TextFormat("  %d\n %s", e->pointsDeVie,e->affichage), X + e->position * TAILLE_CASE + 10, Y + (e->ligne - 1) * TAILLE_CASE + 5, 20, WHITE);
            e = e->next;
        }
        
        // Affichage du score et de la cagnotte avec un fond et une police plus visible
        DrawRectangle(10, 830, 200, 60, BLACK);
        DrawText(TextFormat("Score: %d", jeu->score), 20,830, 20, WHITE);
        //DrawText(TextFormat("Cagnotte: %d", jeu->cagnotte), 20, SCREEN_HEIGHT - 40, 20, WHITE);
        sleep(1);
        EndDrawing();

        jeu->tour+=1;
        actionsTourelles(jeu);//on actionne les tourelles
        Etudiant *courant1=jeu->etudiants;
        //on boucle sur tout les étudiants en les faisants avancer
        while(courant1!=NULL){
            if( courant1->tour<=jeu->tour){//si l'étudiant courant est sur le terrain de jeu ou peut y rentrer
                Etudiant * tmp=courant1;
                /*on utilise une variable temporaire pour pouvoir passer au prochain étudiant
                        sur la ligne avant d'appeler avancer qui risque d'appeler à son tour
                        touche_Etudiant qui va libérer la mémoire allouée par l'étudiant si il meurt
                */
                courant1=courant1->next;
                avancer(tmp,jeu);
            }else{
                courant1=courant1->next;
            }
        }
    }
    CloseWindow();
    gagner(jeu);
}



/*
affichejeu(Jeu *jeu) affiche les Etudiants avec leurs PV tour par tour, tant qu'ils ne sont pas tous mort ou que le joueur n'a pas perdu*/


//a faire mieux apres perdu et gagner avec un joli affichage

/* perdu() s'occupe de l'affichage en cas de victoire*/
void perdu(Jeu* jeu){
    //system("clear");//si vous souhaiter voir toute les étapes mettez cette ligne en commentaire
    printf("Vous avez perdu gros Neuille\n");
    liberer_tourelle(jeu);
    liberer_etudiant(jeu);
    free(jeu);
}
/* perdu() s'occupe de l'affichage en cas de victoire*/
void gagner(Jeu* jeu){
    ajouter_meilleurs_scores(jeu->score);
    //system("clear");
    liberer_etudiant(jeu);
    liberer_tourelle(jeu);
    free(jeu);
    printf("Vous avez gagné\n");
}

//fonction qui verifie si le fichier est conforme
int fichierConforme(FILE* fichierVague)
{
    //gagnotte conforme ?
    char chaineCagnotte[100] = "";
    int i = 0;
    fgets(chaineCagnotte, 100, fichierVague);

    while ((chaineCagnotte[i] != '\0') && (chaineCagnotte[i] != '\n'))
    {

        if ((chaineCagnotte[i] < 48) || (chaineCagnotte[i] > 57))
        {
            printf("Cagnotte non conforme\n"); //les valeurs ascii des chiffres de 0 à 9 sont entre 48 et 57 inclus
            return 0; //0 signifie faux donc ici fichier NON conforme
        }
        i++;
    }
    
    //positions conformes ?
    char chainePositions[100] = "";
    while (fgets(chainePositions, 100, fichierVague) != NULL)
    {
        if (strlen(chainePositions) > 6)
        {
            printf("Format des lignes non conforme.\n");
            return 0;
        }
        else if ((chainePositions[0] < 49) || (chainePositions[0] > 57) || (chainePositions[2] < 49) || (chainePositions[2] > 57))
        {
            printf("Positions non conformes.\n");
            return 0;
        }
        else if ((chainePositions[1] != ' ') || (chainePositions[3] != ' '))
        {
            printf("Espaces entre les positions non conformes.\n");
            return 0;
        }
        else if ((chainePositions[4] != 'K') && (chainePositions[4] != 'Z') && (chainePositions[4] != 'M') && (chainePositions[4] != 'D') && (chainePositions[4] != 'S') && (chainePositions[4] != 'X'))
        {
            printf("Type non conforme.\n");
            return 0;
        }
    }
    printf("Fichier conforme.\n");
    rewind(fichierVague); //si tout va bien, remettre le curseur au debut du fichier
    return 1;
}
/* le tri par insertion en ordre decroissant est utilié dans meilleur_score*/
void tri_par_insertion(int tab[], int taille) {
    for (int i=1;i<taille;i++) {
        int cle=tab[i];
        int j=i-1;

        // Déplacer les éléments du tableau[0..i-1] plus grands que cle
        while (j>= 0 && tab[j]< cle) {
            tab[j+1] = tab[j];
            j--;
        }
        tab[j+1] = cle;
    }
}




void ajouter_meilleurs_scores(int nv_score){
    FILE *fichier = fopen("meilleur_score.txt", "r");
    
    int taille=10;
    int tab[10]={-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    char debut[20];
    fgets(debut,sizeof(debut),fichier);//on saute la première ligne ou il y a la cagnotte.
    int score;
    int i=0;
    int nombre_scores=0;
    while (fscanf(fichier, "%d", &score) == 1) {//on insère les scores enregistrés dans les parties précédentes dans un tableau temporaire
        tab[i]=score;
        nombre_scores+=1;
        i+=1;
    }
    fclose(fichier);//on le ferme pour le rouvrir ensuite
    if(nombre_scores<10){
        for(int i=0;i<taille;i++){//si on a pas 10 meilleurs scores
            if(tab[i]==-1){
                tab[i]=nv_score;
                break;
            }
        }
    }
    else{
        if(tab[taille-1]<nv_score){//sinon on l'ajoute seulement si le nouveau score est meilleur que le moins bon score du fichier
            tab[taille-1]=nv_score;
        }
    }
    tri_par_insertion(tab,taille);//on tri les scores
    fichier= fopen("meilleur_score.txt", "w");
    //on rerempli le fichier avec les nouveaux scores
    fprintf(fichier, "%s", debut);
    for(int i=0;i<taille;i++){
        if(tab[i]!=-1){
        fprintf(fichier, "%d\n", tab[i]);
        }
    }
    fclose(fichier);
}
    
void afficher_meilleurs_scores(){
    FILE *fichier;
    fichier= fopen("meilleur_score.txt", "r");
    char choix[10];
    printf("Voulez vous regarder vos meilleurs scores?, (OUI si vous voulez sinon tapez n'importe quoi)\n");
    scanf("%9s", choix);
    if (strcmp(choix,"OUI")!=0){//si le joueur ecrit OUI, il a fini de placer les tourelles sinon on continue.
        if(!fichier){ //on crée le fichier si il n'existe pas
        fichier= fopen("meilleur_score.txt", "w");
        fputs("Meilleurs scores: \n", fichier);
        }
        fclose(fichier);
        return;
    }
    int score;
    if(!fichier){//on crée le fichier si il n'existe pas
        printf("Il n 'y a pas de scores enregistré\n");
        fichier= fopen("meilleur_score.txt", "w");
        fputs("Meilleurs scores: \n", fichier);
        fclose(fichier);
        return;
    }
    char debut[20];
    fgets(debut,sizeof(debut),fichier);//on saute la première ligne ou il y a la cagnotte.;
    while (fscanf(fichier, "%d", &score) == 1) {//on insère les scores enregistrés dans les parties précédentes dans un tableau temporaire
        printf("%d\n",score);
    }
    fclose(fichier);
}


