#ifndef TOURELLE_H
#define TOURELLE_H


#include <stdio.h>
#include <stdlib.h>
#include <time.h>



#define NOMBRE_POSITIONS 15

typedef struct jeu Jeu;
typedef struct etudiant Etudiant;
typedef struct tourelle {
    int type;
    int pointsDeVie;
    int ligne;
    int position;
    int prix;
    int degats;
    struct tourelle *next;  // Pointeur vers la tourelle suivante
    char* affichage;
} Tourelle;


Tourelle *creerTourelle(int ligne, int position, int type,Jeu* jeu);


void attaquerT(Jeu *jeu, Tourelle *tourelle);

void ralentirEnnemi(Jeu *jeu, Tourelle *tourelle);


void actionsTourelles(Jeu *jeu);

void attaquerZone(Jeu *jeu, Tourelle *tourelle);

void chainage_tourelle(Jeu * jeu);

void toucher_Tourelle(Jeu *jeu,Tourelle* tourelle,int degat);

Tourelle *trouver_pos_exacte_tour(Jeu* jeu,int ligne ,int pos);

void liberer_tourelle(Jeu* jeu);


#endif