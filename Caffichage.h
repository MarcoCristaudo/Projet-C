#ifndef CAFFICHAGE_H
#define CAFFICHAGE_H

#include <stdio.h>
#include <stdlib.h>
#include "struct_jeu.h"

void afficheTerrainNeutre(void);
void afficheVagues(FILE*);
void affichejeu(Jeu *j);
void perdu(Jeu* jeu);
void gagner(Jeu* jeu);
int fichierConforme(FILE *fichierVague);
void tri_par_insertion(int tab[], int taille);
void afficher_meilleurs_scores();
void ajouter_meilleurs_scores(int nv_score);
#endif 
