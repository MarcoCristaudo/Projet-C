#include "Tourelle.h"
#include "struct_jeu.h"
#include "struct_etud.h"
#include <string.h>
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
        nouvelleTourelle->pointsDeVie = 3;
        nouvelleTourelle->type='T';
        nouvelleTourelle->degats=1;
        break;
    case 2: //tourelle_ralentisseuse:
        nouvelleTourelle->prix = 150;
        nouvelleTourelle->pointsDeVie = 5;
        nouvelleTourelle->type='{';
        nouvelleTourelle->degats=1;
        break;
    case 3: //tourelle_mine:
        nouvelleTourelle->prix = 100;
        nouvelleTourelle->pointsDeVie = 1;
        nouvelleTourelle->type='*';
        nouvelleTourelle->degats=10;
        break;
    case 4: //tourelle_zone:
        nouvelleTourelle->prix = 200;
        nouvelleTourelle->pointsDeVie = 7;
        nouvelleTourelle->type='<';
        nouvelleTourelle->degats=2;
        break;
    case 5: //tourelle_mur:
        nouvelleTourelle->prix = 50;
        nouvelleTourelle->pointsDeVie = 10;
        nouvelleTourelle->type='|';
        nouvelleTourelle->degats=0;
        break;
    }
    return nouvelleTourelle;
}
/* chainage_tourelle(Jeu * jeu) demande a l'utilisateur de choisir les tourelles qu'il veut acheter et les places dans une liste chainée dans jeu*/
void chainage_tourelle(Jeu * jeu){

    jeu->nombre_tourelles=0;
    jeu->tourelles=NULL;
    int fini=0;
    Tourelle * tab[NOMBRE_LIGNES][15];
    memset(tab,0,sizeof(tab));
    while (jeu->cagnotte>50 && !fini){//tant que le joueur a de l'argent ou qu'il décide s'arreter, on ajoute des tourelles
        int i;//dans i va etre stocker le type de tourelle
        int ligne;
        int position;
        int temp;
        printf("il vous reste %d crédits\n\n",jeu->cagnotte);
        printf("Quelle type de tourelle voulez vous?\n\n");
        printf("tapez 1 pour placer une tourelle de base. PRIX 100\n\n");
        printf("tapez 2 pour placer une tourelle ralentisseuse. PRIX 150\n\n");
        printf("tapez 3 pour placer une mine. PRIX 100\n\n");
        printf("tapez 4 pour placer une tourelle avec degat de zone. PRIX 200\n\n");
        printf("tapez 5 pour placer un mur. PRIX 50\n\n");

        temp=scanf("%d" ,&i);//vérification d'une saisie valide 
        
        while (temp==EOF || temp!=1 || i <1 || i >5){
        printf("Vous avez saisi une valeur invalide. Veuillez saisir une valeur entre 1 et 5. \n");
        while (getchar() != '\n');  // pour vider le \n dans le buffer sinon on a une boucle infinie
        temp=scanf("%d", &i); 
        }

        while(i!=1 && i!=2 && i!=3 && i!=4 && i!=5 ){//tant que i n'est pas un type qui existe, on redemande à l'utilisateur d'écrire le type qu'il veut
            printf("Ce type de tourelle n'existe pas veuillez chosir correctement\n\n");
            printf("il vous reste %d crédits\n\n",jeu->cagnotte);
            printf("Quelle type de tourelle voulez vous?\n");
            printf("tapez 1 pour placer une tourelle de base. PRIX 100\n\n");
            printf("tapez 2 pour placer une tourelle ralentisseuse. PRIX 150\n\n");
            printf("tapez 3 pour placer une mine. PRIX 100\n\n");
            printf("tapez 4 pour placer une tourelle avec degat de zone. PRIX 200\n\n");
            printf("tapez 5 pour placer un mur. PRIX 50\n\n");
            scanf("%d" ,&i);
        }
        int pas_assez=1;//pas_assez=1 signifie qu'on manque de crédit pour acheter une certaine défense
        while((pas_assez)){
            pas_assez=0;//on l'initialise a 0
            //le switch verifie que l'argent restant n'est pas inférieur au prix de la tourelle de type i. Si c'est le cas, pas_assez=1
            switch(i){
                case 1:
                    if (jeu->cagnotte<100){
                        pas_assez=1;
                    }
                    break;
                case 2:
                    if (jeu->cagnotte<150){
                        pas_assez=1;
                    }
                    break;
                case 3:
                    if (jeu->cagnotte<100){
                        pas_assez=1;
                    }
                    break;
                case 4:
                    if (jeu->cagnotte<200){
                        pas_assez=1;
                    }
                    break;
                case 5:
                    if (jeu->cagnotte<50){
                        pas_assez=1;
                    }
                    break;
            }
            if (pas_assez){//si on a pas assez on redemande au joueur et on reboucle
                printf("Vous n'avez pas assez\n\n");
                printf("il vous reste %d crédits\n\n",jeu->cagnotte);
                printf("Quelle type de tourelle voulez vous?\n\n");
                printf("tapez 1 pour placer une tourelle de base. PRIX 100\n\n");
                printf("tapez 2 pour placer une tourelle ralentisseuse. PRIX 150\n\n");
                printf("tapez 3 pour placer une mine. PRIX 100\n\n");
                printf("tapez 4 pour placer une tourelle avec degat de zone. PRIX 200\n\n");
                printf("tapez 5 pour placer un mur. PRIX 50\n\n");
                scanf("%d" ,&i);
            }
        }
        
        int temp_ligne,temp_position;

        printf("choisissez une ligne entre 1 et 7\n");
        temp_ligne=scanf("%d" ,&ligne);

        while (temp_ligne==EOF || temp_ligne!=1 || ligne <1 || ligne >7){

        printf("Vous avez saisi une valeur invalide. Veuillez saisir une valeur entre 1 et 7. \n");
        while (getchar() != '\n');  // pour vider le \n dans le buffer sinon on a une boucle infinie
        temp_ligne=scanf("%d", &ligne); 

        }

        printf("choisissez une position entre 0 et 13\n");
        temp_position=scanf("%d" ,&position);

        while (temp_position==EOF || temp_position!=1 || position<0 || position >13){

        printf("Vous avez saisi une valeur invalide. Veuillez saisir une valeur entre 0 et 13. \n");
        while (getchar() != '\n');  // pour vider le \n dans le buffer sinon on a une boucle infinie
        temp_position=scanf("%d", &position); 

        }



        int position_occupe=0;
        if (jeu->tourelles!=NULL){
            if(jeu->tourelles==jeu->derniere){
                if(jeu->tourelles->ligne==ligne && jeu->tourelles->position==position){
                    position_occupe=1;//si le joueur a déja placé une tourelle à la ligne et position choisis, on m.a.j position_occupe
                }
            }else{
                Tourelle * courant=jeu->tourelles;
                while(courant!=jeu->derniere){//on parcourt la liste chainée de tourelles
                    if((courant->ligne==ligne) && (courant->position==position)){
                        position_occupe=1;//si on trouve une tourelle à la ligne et à la position choisi, on m.a.j position_occupe
                    }
                    courant=courant->next;
                }
            }
        }
        while(ligne < 1 || ligne > 7 || position < 0 || position > 13 || position_occupe){//on boucle tant que notre tourelle n'est pas placée correctement
            position_occupe=0;
            printf("position invalide\n\n");
            printf("choisissez une ligne entre 1 et 7\n\n");
            scanf("%d" ,&ligne);
            printf("choisissez une position entre 0 et 13\n\n");
            scanf("%d" ,&position);
            if (jeu->tourelles!=NULL){
                if(jeu->tourelles==jeu->derniere){
                    if(jeu->tourelles->ligne==ligne && jeu->tourelles->position==position){
                        position_occupe=1;
                    }
                }else{
                    Tourelle * courant=jeu->tourelles;
                    while(courant!=jeu->derniere){
                        if(courant->ligne==ligne && courant->position==position){
                            position_occupe=1;
                        }
                        courant=courant->next;
                    }
                }
            }
        }
        //Si on a surmonté tout les problèmes possibles précedent, on ajoute la nouvelle tourelle aux autre tourelles
        Tourelle* t=creerTourelle(ligne,position,i,jeu);
        jeu->nombre_tourelles+=1;
        
        if (jeu->tourelles==NULL) {
            jeu->tourelles=t;
            jeu->derniere=t;
            t->next=NULL; // Le tout premier, et donc dernier, a next = NULL
        } else {
            jeu->derniere->next=t;
            jeu->derniere=t;
            t->next=NULL;
        }
        jeu->cagnotte-=t->prix;//on enlève le prix de notre tourelle à la cagnotte
        tab[t->ligne-1][t->position]=t;//on place notre tourelle dans un tableau temporaire.
        for (int i = 0; i < NOMBRE_LIGNES; i++){/*on affiche la tourelle en utilisant un double tableau temporaire*/
            printf("%d|",i+1);
            for (int j = 0; j < 15; j++){
                if(tab[i][j]!=NULL){
                        printf("%3d%c ",tab[i][j]->pointsDeVie,tab[i][j]->type);
                }else{
                    printf("%4c ", '.');
                }
            }
            printf("\n");
        }
        char choix[10];
        printf("Voulez vous vous arréter, il vous reste %d crédits (OUI si vous voulez sinon tapez n'importe quoi)\n",jeu->cagnotte);
        scanf("%9s", choix);
        if (strcmp(choix,"OUI")==0){//si le joueur ecrit OUI, il a fini de placer les tourelles sinon on continue.
            break;
        }
    }
    
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
            //Etudiant *e=trouver_pos_exacte_etu(jeu,tourelle->ligne,tourelle->position+1);//on trouve l'étudiant qui à marché sur la mine
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
