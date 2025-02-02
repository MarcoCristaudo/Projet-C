#include "struct_etud.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "struct_jeu.h"
#include "Tourelle.h"
#include "Caffichage.h"
#include <string.h>

/*creer_etu(int type,int ligne,int tour,Jeu* jeu ,FILE* nom_fichier ) créer des Etudiants et les chaines entre eux en fonctions de leurs lignes
  .Cette fonction est utilisé dans la fonction placer(Jeu* jeu,FILE* nom_fichier)*/
Etudiant* creer_etu(int type,int tour,int ligne,Jeu* jeu ,FILE* nom_fichier){
    Etudiant* e=malloc(sizeof(Etudiant));
    if(e==NULL){// si il y'a une erreur d'allocation la mémoire est libéré correctement
        printf("erreur d'allocation");
        liberer_etudiant(jeu);
        liberer_tourelle(jeu);
        free(jeu);
        fclose(nom_fichier);
        exit(1); //j'arrete tout le programme
    }
    e->type=type;
    type_Etudiant(type,e,jeu);//type_Etudiant est appelé pour initialiser les attributs degats, pointsDeVie et vitesse  
    e->next=NULL;
    e->next_line=NULL;
    e->prev_line=NULL;
    e->prev=NULL;
    e->ligne=ligne;
    e->tour=tour;
    e->position=15;
    return e;
}
/* la fonction type_Etudiant(int type,Etudiant* e,Jeu* j) permet d'initialiser les attributs degats, pointsDeVie
   et vitesse en fonction de l'attribut type d'Etudiants. */
void type_Etudiant(int type,Etudiant* e,Jeu* j){
    
    switch(type){
        case 'Z':
            e->degats=1;
            e->pointsDeVie=5;
            e->PV_max=5;
            e->vitesse=1;
            e->affichage="  o}\n  >-§\n _/_|";
            break;
        case 'M':
            e->degats=3;
            e->pointsDeVie=9;
            e->PV_max=9;
            e->vitesse=1;
            e->un_sur_2=1;
            e->affichage="__o \n    (.)\n _/_|";
            break;
            
        case 'D':
            e->degats=1;
            e->pointsDeVie=4;
            e->PV_max=4;
            e->un_sur_2=1;
            e->vitesse=1;
            e->affichage="<+>\n _0\n /|";
            break;
        case 'S':
            e->degats=1;
            e->pointsDeVie=3;
            e->PV_max=3;
            e->vitesse=4;
            e->affichage=">°\n _§_\n";
            break;
        
        case 'X':
            e->degats=rand()%6+1;
            e->pointsDeVie=rand()%8+1;
            e->PV_max=e->pointsDeVie;
            e->vitesse=rand()%3+1;
            e->affichage="_<?>\n    #>\n    /|";
            break;

        case 'K':
            e->degats=10;
            e->pointsDeVie=2;
            e->PV_max=2;
            e->vitesse=3;
            e->affichage="    o\n   <| ! |>\n   oooo";
            /* il n'y a pas besoin de mettre un cas default car les types d'Etudiant sont déja vérifié
            dans la fonction fichierConforme de Caffichage */
    }
}
/* la fonction placer(Jeu* jeu,FILE* nom_fichier) chaine doublement les Etudiant
    dans jeu en lisant le fichier contenant les informations par rapport aux vagues. Elle utilise creer_etu(int type,int ligne,int tour,Jeu* jeu,FILE* nom_fichier )
    pour ajouter ces Etudiants.   */
void placer(Jeu *jeu,FILE* nom_fichier){

jeu->nombre_etudiant=0;
int tour,ligne;
char type;
int header;
srand(time(NULL));//j'intialise le generateur aleatoire pour le type 'X'
fscanf(nom_fichier, "%d", &header);//on saute la première ligne ou il y a la cagnotte.
fscanf(nom_fichier, "%d %d %c", &tour, &ligne, &type);
Etudiant* e=creer_etu(type,tour,ligne,jeu,nom_fichier);
jeu->nombre_etudiant+=1;
jeu->etudiants=e;
jeu->dernier=e;
jeu->etudiants->next=NULL;;

while (fscanf(nom_fichier, "%d %d %c", &tour, &ligne, &type) == 3) {//tant que le fichier est de la forme tour ligne type on boucle
    jeu->dernier->next=creer_etu(type,tour,ligne,jeu,nom_fichier);//on chaine doublement le nouveau étudiants avec l'ancien dernier
    jeu->dernier->next->prev=jeu->dernier;
    jeu->dernier=jeu->dernier->next; 
    jeu->nombre_etudiant+=1;
    }

}
/* connecte_ligne(Jeu* jeu) permet de doublement chainer les étudiants entre eux par rapport à leur ligne*/

void connecte_ligne(Jeu* jeu){
    for (int i=1;i<=NOMBRE_LIGNES;i++){
        Etudiant* courant=jeu->etudiants;
        while(courant!=jeu->dernier && courant->ligne !=i){//on cherche le premier étudiants sur la ligne i
            courant=courant->next;
        }
        if(courant->next==NULL){//il n'y a pas d'étudiants a la ligne i
            continue;
        }else{
            Etudiant* dernier_ligne=courant; // l'etudiant dernier_ligne pointe sur le dernier etudiants trouvé sur la ligne i
            courant=courant->next;
            while(courant!=jeu->dernier){
                if (courant->ligne==dernier_ligne->ligne){
                    dernier_ligne->next_line=courant; //on chaine les deux étudiants sur la même ligne
                    courant->prev_line=dernier_ligne;
                    dernier_ligne=courant; //le nouveau étudiant trouvé sur la ligne i devient dernier_ligne
                    courant=courant->next; //on continue de parcourir la liste chainée
                }else{
                    courant=courant->next;
                }
            }
            if(jeu->dernier->ligne==dernier_ligne->ligne){//on fait le cas du dernier étudiant
                dernier_ligne->next_line=jeu->dernier;
                jeu->dernier->prev_line=dernier_ligne;
            }
        }
    }
}



/* liberer_liste(Jeu *l) permet de liberer la memoire allouée par la liste chainée d'étudiants */
void liberer_etudiant(Jeu* j){
    if(j->nombre_etudiant!=0){
        Etudiant *courant=j->etudiants;
        while(courant!=j->dernier){
        Etudiant* temp=courant;
        courant=courant->next;
        free(temp);
        }
        free(courant);
    }
}
/* touche_Etudiant(Etudiant *e, int degat_tourelle,int ligne ,Jeu * jeu) permet  de prendre en compte
    les dégats des tourelles et, dans le cas ou l'Etudiant n'a plus de Points de vie, de le tuer. */
void touche_Etudiant(Etudiant *e, int degat_tourelle, int ligne, Jeu *jeu){
    e->pointsDeVie-=degat_tourelle;
    if (e->pointsDeVie<=0) {//si l'étudiant meurt, on l'enlève de la liste chainée
        if (e->prev!=NULL) { //si ce n'est le premier de la chaine
            e->prev->next=e->next;
        }else if (jeu->etudiants==e){ //si c'est le premier de la liste
            jeu->etudiants=e->next; 
        }
        if (e->next!=NULL) {//si ce n'est pas le dernier de la liste
            e->next->prev=e->prev;
        } else if (jeu->dernier==e) {//si c'est le dernier de la liste
            jeu->dernier=e->prev;
        }
        if (e->prev_line!=NULL) {//si ce n'est pas le premier de sa ligne
            e->prev_line->next_line=e->next_line;
        }
        if (e->next_line!=NULL) {//si ce n'est pas le dernier de sa ligne
            e->next_line->prev_line=e->prev_line;
        }
        jeu->nombre_etudiant-=1;
        if(e->type=='K'){//si c'est le type kamikaze, son explosion on doit faire des dégats autour du zombie
            if(e->ligne==1){//on fait le cas de si le kamikaze est sur la première ligne,on va donc pas chercher d'étudiant ou tourelle au dessus
                Etudiant * e1=trouver_pos_exacte_etu(jeu,e->ligne+1,e->position);//on cherche l'étudiant juste en dessous de e
                if(e1&&e1->position<15){//on vérifie s'il existe et s'il est sur le terrain de jeu
                    touche_Etudiant(e1,e->degats,e->ligne,jeu);
                }
                Tourelle *t1=trouver_pos_exacte_tour(jeu,e->ligne+1,e->position);//il peut il y a voir une tourelle si il n y a pas d étudiants
                if(t1&&t1->position<15){//on vérifie s'il existe et s'il est sur le terrain de jeu
                    
                    toucher_Tourelle(jeu,t1,e->degats);
                }
                if(e->position<13){//on vérifie qu'il peut y a voir quelq'un après e
                    Etudiant * e3=trouver_pos_exacte_etu(jeu,e->ligne+1,e->position+1);
                    if(e3&&e3->position<15){
                        touche_Etudiant(e3,e->degats,e->ligne,jeu);//on lui inflige des dégats
                    }
                    Tourelle *t2=trouver_pos_exacte_tour(jeu,e->ligne+1,e->position+1);//il peut il y a voir une tourelle si il n y a pas d étudiants
                    if(t2&&t2->position<15){//on vérifie s'il existe et s'il est sur le terrain de jeu
                        
                        toucher_Tourelle(jeu,t2,e->degats);
                    }
                }
            }else if(e->ligne==7){
                Etudiant * e1=trouver_pos_exacte_etu(jeu,e->ligne-1,e->position);//on cherche l'étudiant juste en dessous de e
                if(e1&&e1->position<15){
                    touche_Etudiant(e1,e->degats,e->ligne,jeu);
                }
                Tourelle *t1=trouver_pos_exacte_tour(jeu,e->ligne-1,e->position);//il peut il y a voir une tourelle si il n y a pas d étudiants
                if(t1&&t1->position<15){//on vérifie s'il existe et s'il est sur le terrain de jeu
                    
                    toucher_Tourelle(jeu,t1,e->degats);
                }
                if(e->position<13){//on vérifie qu'il peut y a voir quelq'un après e
                    Etudiant * e3=trouver_pos_exacte_etu(jeu,e->ligne,e->position+1);
                    if(e3&&e3->position<15){
                        touche_Etudiant(e3,e->degats,e->ligne,jeu);
                    }
                    Tourelle *t3=trouver_pos_exacte_tour(jeu,e->ligne+1,e->position);//il peut il y a voir une tourelle si il n y a pas d étudiants
                    if(t3&&t3->position<15){//on vérifie s'il existe et s'il est sur le terrain de jeu
                        
                        toucher_Tourelle(jeu,t3,e->degats);
                    }
                }
            }else{
                Etudiant * e1=trouver_pos_exacte_etu(jeu,e->ligne-1,e->position);//on cherche l'étudiant juste en dessous de e
                if(e1&&e1->position<15){
                    touche_Etudiant(e1,e->degats,e->ligne,jeu);
                }
                Tourelle *t1=trouver_pos_exacte_tour(jeu,e->ligne-1,e->position);//il peut il y a voir une tourelle si il n y a pas d étudiants
                if(t1&&t1->position<15){//on vérifie s'il existe et s'il est sur le terrain de jeu
                    
                    toucher_Tourelle(jeu,t1,e->degats);
                }
                if(e->position>0){//on vérifie qu'il peut y a voir quelq'un avant e
                    Etudiant * e2=trouver_pos_exacte_etu(jeu,e->ligne,e->position-1);
                    if(e2&&e2->position<15){
                        touche_Etudiant(e2,e->degats,e->ligne,jeu);
                    }
                    
                }
                if(e->position<13){//on vérifie qu'il peut y a voir quelq'un après e
                    Tourelle *t3=trouver_pos_exacte_tour(jeu,e->ligne,e->position+1);
                    if(t3&&t3->position<15){//on vérifie s'il existe et s'il est sur le terrain de jeu
                        
                        toucher_Tourelle(jeu,t3,e->degats);
                    }
                }
                Etudiant * e4=trouver_pos_exacte_etu(jeu,e->ligne+1,e->position);//on cherche l'étudiant juste au dessus de e
                if(e4&&e4->position<15){
                    touche_Etudiant(e4,e->degats,e->ligne,jeu);
                }
                Tourelle *t4=trouver_pos_exacte_tour(jeu,e->ligne+1,e->position);//il peut il y a voir une tourelle si il n y a pas d étudiants
                if(t4&&t4->position<15){//on vérifie s'il existe et s'il est sur le terrain de jeu
            
                    toucher_Tourelle(jeu,t4,e->degats);
                }
            }
        }
        //en fonction du type d'étudiant et du tour courant, le score est plus ou moins élevé.
    switch(e->type){
        case 'Z':
            jeu->score+=50 +100/jeu->tour;
            break;
        case 'M':
            jeu->score+=200 +100/jeu->tour;
            break;
        case 'S':
            jeu->score+=75 +100/jeu->tour;
            break;
        case 'D':
            jeu->score+=100 +100/jeu->tour;
            break;
        case 'X':
            jeu->score+=150 +100/jeu->tour;
            break;
        case 'K':
            jeu->score+=75 +100/jeu->tour;
            break;
        }
    free(e);
    }
}
/* trouver_ligne(Jeu * jeu,int ligne) permet de retrouver facilement le premier étudiant sur sa ligne*/
Etudiant * trouver_ligne_etu(Jeu * jeu,int ligne){
    Etudiant * courant=jeu->etudiants;
    while(courant!=NULL){
        if (courant->ligne==ligne){
            while(courant->prev_line!=NULL){
                courant=courant->prev_line;
            }
            return courant;
        }else{
            courant=courant->next;
        }
    }
    return NULL;
}
/* trouver_pos_exacte_etu(Jeu * jeu,int ligne,int pos) permet de retrouver facilement l'étudiant a position pos et à la ligne ligne*/
Etudiant * trouver_pos_exacte_etu(Jeu * jeu,int ligne,int pos){
    Etudiant *courant=trouver_ligne_etu(jeu,ligne);
    while(courant!=NULL){
        if(courant->position==pos){
            return courant;
        }
        else{
            courant=courant->next_line;
        }
    }
    return NULL;
}


/* avancer(Etudiant* e) fait avancer les Etudiants en fonction de leur vitesse et de la position
    de l'Etudiant qui le précède ou de la présence d'un obstacle*/
void avancer(Etudiant* e,Jeu*jeu){
    if (e->prev_line==NULL){
        if(e->type=='M'){
            if(e->un_sur_2){//le type M avance une fois sur 2
                int vit=1;
                /* a chaque itération de la boucle, on regarde si il y'a une tourelle a la position de l'etudiant -vit.
                    S'il y en a une on apelle toucher_Tourelle, sinon si on est a la vitesse de e on arrete la boucle,
                    et sinon on incrémente vit et on recommence.
                */
                while(vit<=e->vitesse){
                    Tourelle *t=trouver_pos_exacte_tour(jeu,e->ligne ,e->position-vit);
                    if(t){
                        e->position=e->position -vit +1;//on avance jusqua se positionner juste devant la tourelle
                        toucher_Tourelle(jeu,t,e->degats);
                        return;
                    }else{
                        if(e->vitesse==vit){
                            break;
                        }
                        vit+=1;
                    }
                }
                e->position-=e->vitesse;
                e->un_sur_2=0; /* e n'avancera pas au prochain affichage*/
                }else{
                    e->un_sur_2=1;
                }
        }else{
            int vit=1;
            while(vit<=e->vitesse){
                Tourelle *t=trouver_pos_exacte_tour(jeu,e->ligne ,e->position-vit);
                if(t){
                    e->position=e->position -vit +1;//on avance jusqua se positionner juste devant la tourelle
                    toucher_Tourelle(jeu,t,e->degats);
                    return;
                }else{
                    if(e->vitesse==vit){
                        break;
                    }
                    vit+=1;
                }
            }
            e->position-=e->vitesse;
        }
        
    }else{
    switch (e->type){
        case 'S':
            //Je gère l'avancé de e en regardant la distance de l'étudiant devant lui.
            if (e->position-4>e->prev_line->position){ /* je vérifie si il n'y a pas un autre etudiant qui peut gêner e*/
                e->position-=4;
            }
            if (e->position-3>e->prev_line->position){ 
                e->position-=3;
            }else if(e->position-2>e->prev_line->position){
                e->position-=2;
            }else if(e->position-1>e->prev_line->position){
                e->position-=1;
            }
            break;
        case 'K':
        
            if (e->position-3>e->prev_line->position){ /* je vérifie si il n'y a pas un autre etudiant qui peut gêner e*/
                e->position-=3;
            }else if(e->position-2>e->prev_line->position){
                e->position-=2;
            }else if(e->position-1>e->prev_line->position){
                e->position-=1;
            }
            break;
        case 'X':
            int vit=e->vitesse; /* vu que la vitesse de X peut varier, je suis obligé de faire une boucle*/
            while(vit>0){
                if(e->position-vit>e->prev_line->position){
                    e->position-=vit;
                    break;
                }else{
                    vit-=1;
                }
            }
            break;
        case 'M':
            if (e->position-1>e->prev_line->position){
                if(e->un_sur_2){
                    e->position-=1;
                    e->un_sur_2=0; /* e n'avancera pas au prochain affichage*/
                }else{
                    e->un_sur_2=1;
                }
            }else{
                e->un_sur_2=1;
            }
            break;
        case 'D' :
            if(e->un_sur_2){
                Etudiant *prochain_ligne= e->next_line;
                Etudiant *precede_ligne= e->prev_line;
                /* le docteur soigne toute sa ligne une fois sur deux*/
                while(prochain_ligne!=NULL){//tant qu'il y a des étudiants derriere lui, e leurs rajoute 1 PV
                    if(prochain_ligne->pointsDeVie<prochain_ligne->PV_max){
                        prochain_ligne->pointsDeVie+=1;
                    }
                    prochain_ligne=prochain_ligne->next_line;
                }
                while(precede_ligne!=NULL){//tant qu'il y a des étudiants devant lui, e leurs rajoute 1 PV
                    if(precede_ligne->pointsDeVie<precede_ligne->PV_max){
                        precede_ligne->pointsDeVie+=1;
                    }
                    precede_ligne=precede_ligne->prev_line;
                }
                e->un_sur_2=0;//on remet e->un_sur_2 a 0 pour qu'au prochain tour le docteur ne soigne pas
            }
            else{
                e->un_sur_2=1;//si il n'a pas soigné, on remet e->un_sur_2 a 1 pour qu'il soigne au prochain tour
            }
            if (e->position-1>e->prev_line->position){
                e->position-=1;
            }

            break;
        default :
            if (e->position-1>e->prev_line->position){
                e->position-=1;
            }
            break;
    }
    }
    //on regarde si e est arrivé a la fin du terrain de jeu
    if(e->position<0){
        perdu(jeu);
        exit(1);
        }
}










