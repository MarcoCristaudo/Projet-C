#include <stdio.h>
#include <stdlib.h>
#include "tourelle.h"
#include "struct_etud.h"   // pour touche_Etudiant(...) etc.
#include "struct_jeu.h"    // pour la structure Jeu

/*
    Crée une nouvelle tourelle en mémoire (heap) avec ses attributs de base
    (type, points de vie, ligne, position, prix).
    On ne fait pas encore le chaînage dans cette fonction :
    elle retourne simplement un pointeur prêt à être inséré dans la liste.
*/
Tourelle* creerTourelle(int ligne, int position, int type)
{
    Tourelle *nouvelle = malloc(sizeof(Tourelle));
    if (nouvelle == NULL)         
    {
        printf("Erreur d'allocation mémoire pour la tourelle.\n");
        exit(EXIT_FAILURE);
    }

    nouvelle->type   = type;
    nouvelle->ligne      = ligne;
    nouvelle->position   = position;
    nouvelle->next       = NULL;

    switch (type)
    {      
        case TOURRELE_DE_BASE:
            nouvelle->prix        =PRIX_TOURRELE_DE_BASE;
            nouvelle->pointsDeVie =POINTS_DE_VIE_TOURRELE_DE_BASE;
            break;
              
        case TOURRELE_RALENTISSEUR:
            nouvelle->prix        =PRIX_TOURRELE_RALENTISSEUR;
            nouvelle->pointsDeVie = POINTS_DE_VIE_TOURRELE_RALENTISSEUR;
            break;

        case TOURRELE_MINE:
            nouvelle->prix        =PRIX_TOURRELE_MINE;
            nouvelle->pointsDeVie =POINTS_DE_VIE_TOURRELE_MINE;
            break;

        case TOURRELE_ZONE:
            nouvelle->prix    =PRIX_TOURRELE_ZONE;
            nouvelle->pointsDeVie =POINTS_DE_VIE_TOURRELE_ZONE;
            break;

        case TOURRELE_MUR:
            nouvelle->prix  =PRIX_TOURRELE_MUR;
            nouvelle->pointsDeVie =POINTS_DE_VIE_TOURRELE_MUR;
            break;
              
        default:
            // Normalement jamais atteint si on contrôle bien le type avant
            printf("Type de tourelle inconnu : %d\n", type);
            free(nouvelle);        
            exit(EXIT_FAILURE);
    }                                                  

    return nouvelle;
}
       
/*
 * insère une nouvelle tourelle (paramètre 'nouvelle')
 * à la fin de la liste de tourelles du Jeu (jeu->tourelles).
 */
void insererTourelle(Jeu *jeu, Tourelle *nouvelle)
{
    // Si aucune tourelle dans le jeu, la nouvelle devient la première
    if (jeu->tourelles == NULL)
    {
        jeu->tourelles = nouvelle;
    }
    else
    {
        // Sinon, on parcourt la liste jusqu'à la dernière tourelle
        Tourelle *temp = jeu->tourelles;
        while (temp->next != NULL)
        {
            temp = temp->next;                  
        }
        // On accroche la nouvelle tourelle en bout de liste
        temp->next = nouvelle;   
    }
}

/*
    Retire une tourelle (t) de la liste chaînée puis la libère.
    Utile pour une tourelle "Mine" qui explose, par exemple,
    ou si une tourelle se fait détruire.
*/
void supprimerTourelle(Jeu* jeu, Tourelle* t)
{
    if (t == NULL || jeu->tourelles == NULL)
        return; // Rien à faire

    // Si la tourelle à supprimer est en tête de liste
    if (jeu->tourelles == t)
    {    
        jeu->tourelles = t->next;
        free(t);
        return;
    }

    // Sinon on la cherche dans la liste
    Tourelle* prec = jeu->tourelles;
    while (prec->next != NULL && prec->next != t)
        prec = prec->next;

    if (prec->next == t)  // trouvée
    {
        prec->next = t->next;    
        free(t);
    }
    // si non trouvée, t n’appartient pas à la liste => rien
}

/*
    Fonction d’attaque "classique" :
    - Tourelle de base : inflige un certain nombre de dégâts au premier ennemi de la même ligne
    - On parcourt la liste d'ennemis ; dès qu'on en trouve un sur la bonne ligne et position <= 14
      on lui inflige des dégâts puis on quitte (une seule cible).
*/
void attaquerLigne(Jeu *jeu, Tourelle *tourelle)
{
    const int degats = 1;
    Etudiant *ennemi = jeu->etudiants;

    while (ennemi)      
    {
        // On vise un ennemi sur la même ligne
        if (ennemi->ligne == tourelle->ligne)
        {
            // Inflige des dégâts et termine l'action
            touche_Etudiant(ennemi, degats, ennemi->ligne, jeu);
            printf(">> Un ennemi de la ligne %d est touché de %d de dégats.\n",
                   tourelle->ligne, degats);
            break;
        }
        ennemi = ennemi->next;
    }
}



/*
    Tourelle ralentisseur :
    - Réduit la vitesse du premier ennemi de la même ligne (sans descendre en dessous de 1)
*/    


void ralentirEnnemi(Jeu *jeu, Tourelle *tourelle)
{
    Etudiant *ennemi = jeu->etudiants;
    while (ennemi)
    {
        if (ennemi->ligne == tourelle->ligne)        {
            if (ennemi->vitesse > 1)
                ennemi->vitesse -= 1;
            printf(">> Un ennemi de la ligne %d est ralenti à la vitesse %d.\n",
                   tourelle->ligne, ennemi->vitesse);
            break; // Une seule cible
        }
        ennemi = ennemi->next;
    }
}

/*
    Tourelle de zone :
    - Inflige des dégâts (ex : 2) à tous les ennemis dont la position est proche de la position de la tourelle
      (par exemple +/- 1 ou +/- 2 selon la portée souhaitée).
*/
void attaquerZone(Jeu *jeu, Tourelle *tourelle)
{
    // On peut choisir la portée, ici on met un rayon de 1
    const int rayon  = 3;
    const int degats = 2;

    Etudiant *ennemi = jeu->etudiants;
    while (ennemi)
    {
        if (ennemi->ligne == tourelle->ligne)
        {
            int diffPos = tourelle->position - ennemi->position;
            if (diffPos < 0) diffPos = -diffPos;  // valeur absolue

            // S'il est dans le rayon
            if (diffPos <= rayon)
            {
                touche_Etudiant(ennemi, degats, ennemi->ligne, jeu);
                printf(">> Un ennemi de la ligne %d est touché de %d de dégats.\n",
                       tourelle->ligne, degats);
            }
        }
        ennemi = ennemi->next;
    }
}

/*
    Tourelle mine :
    - Si un ennemi se trouve exactement sur la position (ligne/position identiques),
      la mine explose : on tue l'ennemi instantanément et on supprime la mine.
    - Sinon, la mine n'attaque pas "à distance" : elle attend qu’un ennemi marche dessus.

    Remarque : on peut appeler cette fonction chaque tour, AVANT que les ennemis se déplacent,
    pour voir si un ennemi est au contact. Ou BIEN juste après le déplacement.
*/
void gererMine(Jeu *jeu, Tourelle *tourelle)
{
    Etudiant* ennemi = jeu->etudiants;
    while (ennemi)
    {
        if (ennemi->ligne == tourelle->ligne && ennemi->position == tourelle->position)
        {
            printf(">> Mine explosée en ligne %d, position %d!\n", tourelle->ligne, tourelle->position);
            // On tue l'ennemi
            // (on peut appeler touche_Etudiant avec un gros dégat suffisant pour le tuer)
            touche_Etudiant(ennemi, ennemi->pointsDeVie, ennemi->ligne, jeu);
            printf(">> Un ennemi de la ligne %d est touché de %d de dégats.\n",
                   tourelle->ligne, ennemi->pointsDeVie);
            // On supprime la mine elle-même
            supprimerTourelle(jeu, tourelle);
            printf(">> Tourelle de la ligne %d est supprimee.\n",
                   tourelle->ligne);
            return;
        }
        ennemi = ennemi->next;
    }
}

/*
    Regroupe l’action de toutes les tourelles pour un tour donné :
    - On parcourt la liste dans l’ordre de chaînage
    - Chaque tourelle agit selon son type
    - On peut aussi vérifier si la tourelle est détruite (pointsDeVie <= 0) et la retirer.
*/
void actionsTourelles(Jeu *jeu)
{
    Tourelle *courante = jeu->tourelles;
    Tourelle *suivante = NULL;
    while (courante)
    {
        // On sauvegarde le pointeur pour le tour suivant
        // (car si on supprime courante dans le cas d’une mine, on perd le next)
        suivante = courante->next;

        // Vérifier si la tourelle est encore en vie
        if (courante->pointsDeVie <= 0)
        {
            supprimerTourelle(jeu, courante);
            printf(">> Tourelle de la ligne %d est supprimee.\n",
                   courante->ligne);
        }
        else
        {
            // Selon le type, on agit
            switch (courante->type)
            {
                case TOURRELE_DE_BASE:
                    attaquerLigne(jeu, courante);
                    break;

                case TOURRELE_RALENTISSEUR:
                    ralentirEnnemi(jeu, courante);
                    break;

                case TOURRELE_MINE:
                    gererMine(jeu, courante);
                    // si elle explose, elle est déjà supprimée dans gererMine
                    break;

                case TOURRELE_ZONE:
                    attaquerZone(jeu, courante);       
                    break;

                case TOURRELE_MUR:
                    // Ne fait rien offensivement
                    break;

                default:
                    printf("Tourelle inconnue (type=%d)\n", courante->type);
                    break;
            }
        }    

        courante = suivante;
    }
}

/*
    Exemple de fonction pour construire un menu console et insérer les tourelles dans la partie jeu. Faut vérifier avant de construire la tourelle si la place est déJà occupée ou pas;
*/
void construireTourelles(Jeu *jeu)             {
    int choixType = 0;     
    int ligne     = 0;
    int position  = 0;

    while (1)
    {
        printf("\n--- Construire une nouvelle tourelle (cagnotte=%d) ---\n", jeu->cagnotte);
        printf("1) Tourelle de Base (%d) - Faible coût, dégâts sur la ligne\n", PRIX_TOURRELE_DE_BASE);
        printf("2) Ralentisseur   (%d) - Réduit la vitesse d’un ennemi\n", PRIX_TOURRELE_RALENTISSEUR);
        printf("3) Mine           (%d) - Explose au contact\n", PRIX_TOURRELE_MINE);
        printf("4) Zone           (%d) - Frappe en zone autour d’elle\n", PRIX_TOURRELE_ZONE);
        printf("5) Mur            (%d) - Bloque, pas d’attaque\n", PRIX_TOURRELE_MUR);
        printf("0) Terminer\n");
        printf("Votre choix : ");
        scanf("%d", &choixType);
        if (choixType == 0)
            break;  // on sort du menu

        // Vérification du choix
        if (choixType < 1 || choixType > 5)
        {
            printf("Choix invalide.\n");
            continue;
        }

        // Demander ligne et position
        printf("Choisissez la ligne (1-%d) : ", NOMBRE_LIGNES);
        scanf("%d", &ligne);
        printf("Choisissez la position (1-%d) : ", NOMBRE_POSITIONS);
        scanf("%d", &position);

        // Vérification simple
        if (ligne < 1 || ligne > NOMBRE_LIGNES || position < 1 || position > NOMBRE_POSITIONS)
        {
            printf("Position/ligne incorrectes.\n");
            continue;
        }

        // Détermine le prix requis
        int typeTourelle = choixType; // 1 à 5 cf tourelle.h
        int prixRequis   = 0;
        switch (typeTourelle)
        {
            case TOURRELE_DE_BASE:        prixRequis = PRIX_TOURRELE_DE_BASE;       break;
            case TOURRELE_RALENTISSEUR:   prixRequis = PRIX_TOURRELE_RALENTISSEUR;  break;
            case TOURRELE_MINE:           prixRequis = PRIX_TOURRELE_MINE;          break;
            case TOURRELE_ZONE:           prixRequis = PRIX_TOURRELE_ZONE;          break;
            case TOURRELE_MUR:            prixRequis = PRIX_TOURRELE_MUR;           break;
        }

        // Vérifie si le joueur peut payer       
        if (jeu->cagnotte < prixRequis)
        {
            printf("Pas assez d’argent (coût=%d, cagnotte=%d)\n", prixRequis, jeu->cagnotte);
            continue;
        }

        // Création et insertion
        Tourelle* t = creerTourelle(ligne, position, typeTourelle);
        insererTourelle(jeu, t);
        jeu->cagnotte -= prixRequis; // Mise à jour de la cagnotte
        printf("Tourelle placée en ligne %d, position %d. Nouveau solde : %d\n",
               ligne, position, jeu->cagnotte);
    }
}       
