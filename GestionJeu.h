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
PUBLIC char whichChar(int pos, int nbC, int nbJ, int *posCh, char* nomChevaux, int* ptrCouleur);

// Nombre de chevaux restants à l'écurie pour une équipe donnée
PUBLIC int nbHorsesHome(Team t, int nbC, int *posCh);

//Résultat pseudo-aléatoire correspondant au lancé d'un dé.
PUBLIC int lancerDes();

//Affiche un tableau d'entiers sur x lignes et y colones
PUBLIC void afftab (int *tab, int x, int y);

// Applique le choix du joueur
PUBLIC void appliquerChoix(int choix, int numJoueur, int nbCasesAAvancer, int *positionChevaux, int nbC, int nbJ);

// Retourne la première case du joueur passé en paramètre (celle où vont les chevaux en sortant de l'écurie)
PUBLIC int getPremiereCaseJoueur(int numJoueur);

// Retourne la case suivante en gérant le passage de 56 à 1
PUBLIC int getCaseSuivante(int caseCourante, int nbCasesAAvancer);

// Retourne la première case de l'escalier ou la suivante selon la case courante
PUBLIC int getmonterEscalier(int caseCourante, int numJoueur);

// Retourne la première case de l'escalier
PUBLIC int getPremierEscalierJoueur(int numJoueur);

//Renvoie 1 si le joueur numJoueur à gagné, 0 sinon; d'après la position de ses chevaux dans posC
PUBLIC int aGagne (int numJoueur, int nbC ,int* posC);

// Mange un cheval adverse si présent
PUBLIC void mangerAdversaireSiPresent(int numCaseFuture, int* positionCh, int nbC, int nbJ);

//Transforme un vecteur de 4*n caractères en 16 caractères.
PUBLIC void etendreVecteurChoix (char* old, char* new, int nbC);

//Affiche un tableau
PUBLIC void afficher(int *pos , int x, int y);

//Renvoie 1 si aucun cheval n'est sur notre chemin 
PUBLIC int posAvancer(int actuel, int *positions, int nbreChev, int nbreJoueur, int des);

//Affiche un choix
PUBLIC void afficherChoix(char *tab, int x);
PUBLIC void afficherChoixLG(char *tab, int x);

//Renvoie un tableau des choix possibles
PUBLIC void determinerChoix(int nChev, int nbreJoueyr, int joueur, int *position, int des, char *c);

//Renvoie 1 si on peut sortir un cheval de l'écurie
PUBLIC int sortirCheval(int *position, int nbreChev, int joueur);

//Initialise le tableau
PUBLIC void init(char *tab, int x);

//Renvoie 1 si on peut monter dans les escaliers
PUBLIC int monter(int positionPresent, int *position, int joueur, int nbChev);
#undef PUBLIC
#endif	/* GESTION_JEU_H */
