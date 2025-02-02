#include "Tourelle.h"
#include "struct_jeu.h"
#include "struct_etud.h"
#include <string.h>
#include <raylib.h>

#define TAILLE_CASE 100  //taille des cases de la grille
#define Y 150 //on descend de 90 pixel sur l'axe X
#define X 80  //on avance de 90 pixel sur l'axe X
/* creerTourelle(int ligne, int position, int choix_joueur,Jeu *jeu) intialise une tourelle. Cette fonction est utiliser dans chainage_tourelle*/
Tourelle *creerTourelle(int ligne, int position, int choix_joueur,Jeu *jeu){

    Tourelle *nouvelleTourelle = malloc(sizeof(Tourelle));
    if (!nouvelleTourelle)
    {
        printf("Allocation mémoire pour la tourelle échouée");
        exit(EXIT_FAILURE);
    }
    
    nouvelleTourelle->ligne = ligne;
    nouvelleTourelle->position = position;
    nouvelleTourelle->next=NULL;
        
    switch (choix_joueur)//on switch sur la valeur rentré par le joueur dans chainage_tourelle.
    {
    case 1: //tourelle_de_base:
        nouvelleTourelle->prix = 100;
        nouvelleTourelle->pointsDeVie = 5;
        nouvelleTourelle->type='T';
        nouvelleTourelle->degats=2;
        nouvelleTourelle->affichage="[_=>\n  T";
        break;
    case 2: //tourelle_ralentisseuse:
        nouvelleTourelle->prix = 150;
        nouvelleTourelle->pointsDeVie = 5;
        nouvelleTourelle->type='{';
        nouvelleTourelle->degats=1;
        nouvelleTourelle->affichage="|={\n /|";
        break;
    case 3: //tourelle_mine:
        nouvelleTourelle->prix = 100;
        nouvelleTourelle->pointsDeVie = 1;
        nouvelleTourelle->type='*';
        nouvelleTourelle->degats=10;
        nouvelleTourelle->affichage="<ô>";
        break;
    case 4: //tourelle_zone:
        nouvelleTourelle->prix = 200;
        nouvelleTourelle->pointsDeVie = 7;
        nouvelleTourelle->type='<';
        nouvelleTourelle->degats=2;
        nouvelleTourelle->affichage="/|^<\n   |U|";
        break;
    case 5: //tourelle_mur:
        nouvelleTourelle->prix = 50;
        nouvelleTourelle->pointsDeVie = 10;
        nouvelleTourelle->type='|';
        nouvelleTourelle->degats=0;
        nouvelleTourelle->affichage="  |>\n    |>\n    |>";
        break;
    }
    return nouvelleTourelle;
}


void chainage_tourelle(Jeu *jeu) {
    jeu->nombre_tourelles = 0;
    jeu->tourelles = NULL;
    int fini = 0;
    setenv("XMODIFIERS", "@im=none", 1);// on désactive l'IME (un systeme qui nous permet d'écrire des caractères en chinois,japonais etc qui nous sont inutile ici) pour éviter certaines fuites mémoire de raylib
    InitWindow(1700, 1200, "Placement des Tourelles"); // Initialise la fenêtre
    SetTargetFPS(30); // on définit la cadence d'affichage à 30 FPS pour avoir un affichage plus fluide
    static int ligne = 1;
    static int position = 0;
    static int type = 1;
    /* On bouvcle tant que l'on n'a pas fermé la fenètre, appuyer sur ENTER ou que l'on a encore assez pour acheter la tourelle la moins cher*/
    while (!WindowShouldClose() && !fini && jeu->cagnotte >= 50) {
        BeginDrawing();//configure la zone mémoire où le texte est écrit (canvas)
        ClearBackground(DARKGRAY);
        // DrawText permet d'écrire dans la fenètre ouverte
        DrawText("Utilisez les flèches pour vous déplacer, ESPACE pour placer une tourelle, 1-5 pour type, ENTER pour finir", 10, 10, 20, WHITE);
        DrawText(TextFormat("Cagnotte: %d", jeu->cagnotte), 10, 40, 20, WHITE);
        DrawText(TextFormat("Type sélectionné: %d", type), 10, 60, 20, WHITE);
        DrawText("Tapez 1 pour sélectionner une tourelle de base. PRIX 100", 10, 800, 20, WHITE);
        DrawText("Tapez 2 pour sélectionner une tourelle ralentisseuse. PRIX 150", 10, 830, 20, WHITE);
        DrawText("Tapez 3 pour sélectionner une mine. PRIX 100", 10, 860, 20, WHITE);
        DrawText("Tapez 4 pour sélectionner une tourelle avec degat de zone. PRIX 200", 10, 890, 20, WHITE);
        DrawText("Tapez 5 pour sélectionner un mur. PRIX 50", 10, 920, 20, WHITE);
        
        // permet de se déplacer sur la grille
        if (IsKeyPressed(KEY_UP) && ligne > 1) ligne--;
        if (IsKeyPressed(KEY_DOWN) && ligne < NOMBRE_LIGNES) ligne++;
        if (IsKeyPressed(KEY_LEFT) && position > 0) position--;
        if (IsKeyPressed(KEY_RIGHT) && position < 13) position++;
        
        // permet de changer de type de tourelle
        if (IsKeyPressed(KEY_ONE)) type = 1;
        if (IsKeyPressed(KEY_TWO)) type = 2;
        if (IsKeyPressed(KEY_THREE)) type = 3;
        if (IsKeyPressed(KEY_FOUR)) type = 4;
        if (IsKeyPressed(KEY_FIVE)) type = 5;
        
        // On affiche la grille
        
        for (int i = 0; i < NOMBRE_LIGNES; i++) {
            for (int j = 0; j < 15; j++) {
                //On encadre en rouge la case sur laquelle on pointe
                Color couleur=(i + 1 == ligne && j == position) ? RED: LIGHTGRAY;
                DrawRectangleLines(X + j * TAILLE_CASE, Y + i * TAILLE_CASE, TAILLE_CASE, TAILLE_CASE, couleur);
            }
        }
        
        // on affichage les tourelles placées
        Tourelle *t = jeu->tourelles;
        while (t) {
            DrawText(TextFormat("%c\n%d  %s\n", t->type, t->pointsDeVie,t->affichage), X + t->position * TAILLE_CASE + 10, Y + (t->ligne - 1) * TAILLE_CASE + 10, 20, WHITE);
            t = t->next;
        }
        
        EndDrawing();//on ferme le canvas
        
        if (IsKeyPressed(KEY_SPACE)) {
            // on vérifie si une tourelle existe déjà à cet emplacement et si on a assez de crédits
            Tourelle *tourelle = creerTourelle(ligne, position, type, jeu);
            if (!trouver_pos_exacte_tour(jeu, ligne, position)&& jeu->cagnotte>=tourelle->prix) {
                //si on peut placer la tourelle:

                if (jeu->tourelles == NULL) {//si c'est la première
                    jeu->tourelles = tourelle;
                    jeu->derniere = tourelle;
                } else {//sinon on la chaine avec l'ancienne dernière tourelle
                    jeu->derniere->next = tourelle;
                    jeu->derniere = tourelle;
                }
                //on incrémente le nombre de tourelle
                jeu->nombre_tourelles+=1;
                //on enlève le prix de la tourelle
                jeu->cagnotte -= tourelle->prix;
            }else{
                //si la tourelle n'a pas pu etre placé on libère la mémoire allouée
                free(tourelle);
            }
        }
        //si on appuye sur ENTER, on m.a.j la valeur de fini pour pouvoir sortir de la boucle
        if (IsKeyPressed(KEY_ENTER)) {
            fini = 1;
        }
    }
    //on ferme la fenètre lorsque l'on a fini
    CloseWindow();
}
        


/*trouver_pos_exacte_tour(Jeu* jeu,int ligne ,int pos) permet de trouver si elle existe la tourell à la position pos et ligne ligne*/
Tourelle *trouver_pos_exacte_tour(Jeu* jeu,int ligne ,int pos){
    Tourelle* courante=jeu->tourelles;
    while(courante!=NULL){
        if(courante->ligne==ligne && courante->position==pos){
            return courante;
        }
        courante=courante->next;
    }
    return NULL;//si elle n'existe pas on retourne NULL
}
/* toucher_Tourelle(Jeu *jeu,Tourelle* tourelle,int degat) permet de gérer le cas ou un étudiant rentre en contact avec une tourelle*/
void toucher_Tourelle(Jeu *jeu,Tourelle* tourelle,int degat){
    if(tourelle==NULL){
        return;
    }
    tourelle->pointsDeVie-=degat;//on décrémente les PV avec les dégats de l'étudiant
    if(tourelle->pointsDeVie<=0){//si la tourelle est détruite
        Etudiant *e=trouver_pos_exacte_etu(jeu,tourelle->ligne,tourelle->position+1);
        if(e && e->type=='K'){//si e est un kamikaze, il faut le faire exploser
            touche_Etudiant(e ,e->degats,e->ligne,jeu);
        }
        else if(tourelle->type=='*'){//si c'est une mine
            if(e){//si il existe
                touche_Etudiant(e ,tourelle->degats,e->ligne,jeu);//on inflige les dégats de la mine à e
                
            }
        }
        //on enlève la tourelle détruite de la chaine de tourelles
        Tourelle * courante=jeu->tourelles;
        if (tourelle==courante){//si c'est la première
            courante=tourelle->next;
            jeu->tourelles=courante;
        }else{
            while(courante->next!=tourelle){
                courante=courante->next;
            }
            courante->next=tourelle->next;
            if (tourelle==jeu->derniere){//si c'est la dernière
                jeu->derniere=courante;
            }
        }
        jeu->nombre_tourelles-=1;
        free(tourelle);
    }
}
/* attaquerT(Jeu *jeu, Tourelle *tourelle) s'occupe d'infliger les dégats de la tourelle de base au premier étudiant sur sa ligne*/
 void attaquerT(Jeu *jeu, Tourelle *tourelle){
    Etudiant *e=trouver_ligne_etu(jeu,tourelle->ligne);//on cherche le premier étudiant sur la ligne de la tourelle
    if (e!=NULL && e->position<15){//si e existe et qu'il est sur le terrain de jeu
    touche_Etudiant(e,tourelle->degats,tourelle->ligne,jeu);//on lui inflige des dégats
    }
}
/* ralentirEnnemi(Jeu *jeu, Tourelle *tourelle) s'occupe d'infliger les dégats de la tourelle ralentisseuse au premier étudiant sur sa ligne*/
 void ralentirEnnemi(Jeu *jeu, Tourelle *tourelle){
    Etudiant *e=trouver_ligne_etu(jeu,tourelle->ligne);
    if (e!=NULL){//si on a un étudiant sur cette ligne
        while(e->next_line!=NULL&&e->position<15){//on touche et ralenti tous les étudiants sur la ligne
        e->vitesse = (e->vitesse > 1) ? e->vitesse - 1 : 1; // Réduit la vitesse sans tomber en dessous de 1
        Etudiant* temp=e;/*on utilise une variable temporaire pour pouvoir passer au prochain étudiant
                        sur la ligne avant d'appeler touche_Etudiant, pour éviter d'utiliser e que l'on a free dans cette touche_Etudiant
        */
        e=e->next_line;
        touche_Etudiant(temp,tourelle->degats,tourelle->ligne,jeu);//on lui inflige des dégats
        
        }
        if(e->position<15){
            touche_Etudiant(e,tourelle->degats,tourelle->ligne,jeu);//on fait le cas du dernier sur sa ligne;
        }
    }
            
}
/* attaquerZone(Jeu *jeu, Tourelle *tourelle) s'occupe d'infliger les dégats de la tourelle de zone premier étudiant sur sa ligne et
    si ils existent, à celui juste derière lui,au dessus de lui et en dessous de lui*/
void attaquerZone(Jeu *jeu, Tourelle *tourelle){
    Etudiant *e=trouver_ligne_etu(jeu,tourelle->ligne);
    if(e==NULL){//on vérifie si il y a un étudiant sur la ligne de la tourelle
        return;
    }
    if(e->position>=15){//on vérifie si il est sur le terrainde jeu
        return;
    }
    

    if(tourelle->ligne==1){//on fait le cas de si la tourelle est sur la première ligne,on va donc pas chercher d'étudiant au dessus
        Etudiant * e1=trouver_pos_exacte_etu(jeu,tourelle->ligne+1,e->position);//on cherche l'étudiant juste en dessous de e
        if(e1&&e1->position<15){//on vérifie s'il existe et s'il est sur le terrain de jeu
            touche_Etudiant(e1,tourelle->degats,tourelle->ligne,jeu);
        }
        
        if(e->position<13){//on vérifie qu'il peut y a voir quelq'un après e
            Etudiant * e3=trouver_pos_exacte_etu(jeu,tourelle->ligne+1,e->position+1);
            if(e3&&e3->position<15){
                touche_Etudiant(e3,tourelle->degats,tourelle->ligne,jeu);//on lui inflige des dégats
            }
        }
    touche_Etudiant(e,tourelle->degats,tourelle->ligne,jeu);//on lui inflige des dégats
    }
    else if(tourelle->ligne==7){
        Etudiant * e1=trouver_pos_exacte_etu(jeu,tourelle->ligne-1,e->position);//on cherche l'étudiant juste en dessous de e
        if(e1&&e1->position<15){
            touche_Etudiant(e1,tourelle->degats,tourelle->ligne,jeu);
        }
        
        if(e->position<13){//on vérifie qu'il peut y a voir quelq'un après e
        Etudiant * e3=trouver_pos_exacte_etu(jeu,tourelle->ligne,e->position+1);
        if(e3&&e3->position<15){
            touche_Etudiant(e3,tourelle->degats,tourelle->ligne,jeu);
        }
        }
        
        touche_Etudiant(e,tourelle->degats,tourelle->ligne,jeu);//on lui inflige des dégats
    }else{
        Etudiant * e1=trouver_pos_exacte_etu(jeu,tourelle->ligne-1,e->position);//on cherche l'étudiant juste en dessous de e
        if(e1&&e1->position<15){
            touche_Etudiant(e1,tourelle->degats,tourelle->ligne,jeu);
        }
        if(e->position>0){//on vérifie qu'il peut y a voir quelq'un avant e
        Etudiant * e2=trouver_pos_exacte_etu(jeu,tourelle->ligne,e->position-1);
        if(e2&&e2->position<15){
            touche_Etudiant(e2,tourelle->degats,tourelle->ligne,jeu);
        }
        }
        if(e->position<13){//on vérifie qu'il peut y a voir quelq'un après e
        Etudiant * e3=trouver_pos_exacte_etu(jeu,tourelle->ligne,e->position+1);
        if(e3&&e3->position<15){
            touche_Etudiant(e3,tourelle->degats,tourelle->ligne,jeu);
        }
        }
        Etudiant * e4=trouver_pos_exacte_etu(jeu,tourelle->ligne+1,e->position);//on cherche l'étudiant juste au dessus de e
        if(e4&&e4->position<15){
            touche_Etudiant(e4,tourelle->degats,tourelle->ligne,jeu);
        }
        
        touche_Etudiant(e,tourelle->degats,tourelle->ligne,jeu);//on lui inflige des dégats
    }
    
}
/*  actionsTourelles(Jeu *jeu) boucle sur la liste chainée de tourelles 
et effectue les actions de toutes les tourelles du jeu sauf la mine qui est actionné dans toucher_tourelles */
void actionsTourelles(Jeu *jeu)
{
    Tourelle *tourelle = jeu->tourelles;
    while (tourelle != NULL)
    {
        switch (tourelle->type)
        {
        case 'T':
            attaquerT(jeu, tourelle);
            break;
        case '{':
            ralentirEnnemi(jeu, tourelle);
            break;
        case '<':
            attaquerZone(jeu, tourelle);
            break;
        default:
            break;
        }
        tourelle = tourelle->next;
    }
}
/*libère la mémoire allouée par les tourelles restantes*/
void liberer_tourelle(Jeu* j){
        if(j->nombre_tourelles!=0){
        Tourelle *courant=j->tourelles;
        while(courant!=j->derniere){
        Tourelle* temp=courant;
        
        courant=courant->next;
        free(temp);
        }
        free(courant);
        }
    }
