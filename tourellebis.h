#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "struct_jeu.h"

#define NOMBRE_TOURS 9
#define NOMBRE_LIGNES 7
#define NOMBRE_POSITIONS 15

// Types de tourelles
#define TOURRELE_DE_BASE 1
#define TOURRELE_RALENTISSEUR 2
#define TOURRELE_MINE 3
#define TOURRELE_ZONE 4
#define TOURRELE_MUR 5

// Prix tourelles
#define PRIX_TOURRELE_DE_BASE 100
#define PRIX_TOURRELE_RALENTISSEUR 150
#define PRIX_TOURRELE_MINE 80
#define PRIX_TOURRELE_ZONE 250
#define PRIX_TOURRELE_MUR 300

// Points de vie tourelles
#define POINTS_DE_VIE_TOURRELE_DE_BASE 3
#define POINTS_DE_VIE_TOURRELE_RALENTISSEUR 5
#define POINTS_DE_VIE_TOURRELE_MINE 3
#define POINTS_DE_VIE_TOURRELE_ZONE 15
#define POINTS_DE_VIE_TOURRELE_MUR 20

typedef struct tourelle {
    int type;               // Type de la tourelle
    int pointsDeVie;        // Points de vie de la tourelle
    int ligne;              // Ligne de la tourelle
    int position;           // Position de la tourelle
    int prix;               // Coût de la tourelle
    struct tourelle *next;  // Pointeur vers la tourelle suivante
} Tourelle;


// =====================================
//          Fonctions Tourelles
// =====================================

//      Création de Tourelle
Tourelle *creerTourelle(int ligne, int position, int type);

void insererTourelle(Jeu *jeu, Tourelle *nouvelle);

//      Contact Tourelles
void contactTourelles(Jeu *jeu);

//      Attaque Tourelles
void attaquerLigne(Jeu *jeu, Tourelle *tourelle);

//      Ralentire Tourelles
void ralentirEnnemi(Jeu *jeu, Tourelle *tourelle);

//      Action Tourelles
void actionsTourelles(Jeu *jeu);

//      Attaquer Zone
void attaquerZone(Jeu *jeu, Tourelle *tourelle);

//      Place occupée 
void existTourelle(Jeu *jeu, int position, int ligne); // fonction non fait et est à faire dans partie jeu pour qu'on ne puisse pas placer plusieurs tourelles à une même ligne
