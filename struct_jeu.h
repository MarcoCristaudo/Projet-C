#ifndef STRUCT_JEU_H
#define STRUCT_JEU_H



#include <stdio.h>
#include <stdlib.h>

typedef struct tourelle Tourelle;
typedef struct etudiant Etudiant;
typedef struct jeu{ 
    Tourelle* tourelles; 
    Tourelle* derniere; 
    Etudiant* etudiants; 
    Etudiant* dernier;
    int cagnotte; 
    int tour; 
    int nombre_tourelles;
    int nombre_etudiant;
    int score;
    } Jeu;

#endif