/* ---------------------------------------------------------------------------
 * Gestion du jeu des petits chevaux.
 * Auteur(s) : 
 */

#ifndef GESTION_JEU_H
#define GESTION_JEU_H

#ifndef GESTION_JEU_C
#define PUBLIC extern
#else
#define PUBLIC
#endif

// Constantes pour la gestion des équipes
typedef enum {ROUGE = 'R', MAGENTA = 'M', VERTE = 'V', BLEUE = 'B'} Team;

// Retourne une chaîne correspondant à une équipe
PUBLIC char* toString(Team t);

// Détermine le caractère à afficher en fonction de la position sur la grille
// Version pour une grille vide : 
//      1..56 = le tour normal, origine en haut à droite (rouge)
//      61..66 = escalier de l'équipe en haut à droite (rouge)
//      71..76 = escalier de l'équipe en bas à droite (magenta)
//      81..86 = escalier de l'équipe en bas à gauche (vert)
//      91..96 = escalier de l'équipe en haut à gauche (bleu)
PUBLIC char whichChar(int pos);

// Nombre de chevaux restants à l'écurie pour une équipe donnée
PUBLIC int nbHorsesHome(Team t);

//Résultat pseudo-aléatoire correspondant au lancé d'un dé.
PUBLIC int lancerDes();

//Affiche un tableau d'entiers sur x lignes et y colones
PUBLIC void afftab (int **tab, int x, int y);

// Applique le choix du joueur
PUBLIC void appliquerChoix(int choix, int numJoueur, int nbCasesAAvancer, int **positionChevaux);

// Retourne la première case du joueur passé en paramètre (celle où vont les chevaux en sortant de l'écurie)
PUBLIC int getPremiereCaseJoueur(int numJoueur);

// Retourne la case suivante en gérant le passage de 56 à 1
PUBLIC int getCaseSuivante(int caseCourante, int nbCasesAAvancer);

// Retourne la première case de l'escalier ou la suivante selon la case courante
PUBLIC int getmonterEscalier(int caseCourante, int numJoueur);

// Retourne la première case de l'escalier
PUBLIC int getPremierEscalierJoueur(int numJoueur);


#undef PUBLIC
#endif	/* GESTION_JEU_H */
