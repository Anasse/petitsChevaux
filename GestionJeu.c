/* ---------------------------------------------------------------------------
 * Test de l'affichage du plateau pour le jeu des petits chevaux en mode texte
 * Auteur     : Damien Genthial, modifié par Pauze, Chomarat, Berahab et Goudian
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GESTION_JEU_C
#include "GestionJeu.h"

#define ParadisChevaux 101

void appliquerChoix(int choix, int numJoueur, int nbCasesAAvancer, int **positionChevaux);
int getPremiereCaseJoueur(int numJoueur);
int getCaseSuivante(int caseCourante, int nbCasesAAvancer);
int getmonterEscalier(int caseCourante, int numJoueur);
int getPremierEscalierJoueur(int numJoueur);

// Retourne une chaîne correspondant à une équipe
char* toString(Team t) {
    char* res;
    switch (t) {
        case ROUGE: res = "Rouge"; break;
        case MAGENTA: res = "Magenta"; break;
        case VERTE: res = "Verte"; break;
        case BLEUE: res = "Bleue"; break;
    }
    return res;
}

// Détermine le caractère à afficher en fonction de la position sur la grille
// Version pour une grille (presque) vide : 
//      1..56 = le tour normal, origine en haut à droite (rouge)
//      61..66 = escalier de l'équipe en haut à droite (rouge)
//      71..76 = escalier de l'équipe en bas à droite (magenta)
//      81..86 = escalier de l'équipe en bas à gauche (vert)
//      91..96 = escalier de l'équipe en haut à gauche (bleu)
char whichChar(int pos, int nbC, int nbJ, int **posCh) {
	int i=0;
	int j=0;
	int ret = 5;
	while(i<nbJ){
		j=0;
		while(j<nbC && posCh[i*nbC+j]!=pos){
			j++;
		}
		if(j<nbC){ret = i;}
		i++;
	}
	switch(ret){
		case 1 :
			return ROUGE;
			break;
		case 2 :
			return MAGENTA;
			break;
		case 3 :
			return VERTE;
			break;
		case 4 :
			return BLEUE;
			break;
	}
    if (pos <= 56) {
        if ((pos % 14) == 1)
            return '=';
        else return '.';
    } else {
        return '0' + (pos % 10);
    }
}

// Nombre de chevaux restants à l'écurie pour une équipe donnée
int nbHorsesHome(Team t, int nbC, int **posCh) {
	int equipe;
    if(t==ROUGE) {	equipe = 0;
	}else if(t==MAGENTA){ equipe = 1;
	}else if(t==VERTE){ equipe = 2;
	}else if(t==BLEUE){ equipe = 3;}

	int ret = 0;
	int i = 0;
	while(i<nbC){
		if(posCh[equipe*nbC+i] >= 67+10*equipe && posCh[equipe*nbC+i] <= 67+10*equipe+3) ret++;
		i++;
	}

	return ret;
}

/* 
 * Entrées : aucune
 * Sortie : numéro aléatoire entre 1 et 6 pour simuler un dé
 */
int lancerDes(){
	srand(time(NULL));
	int resultat = (rand()%6 +1);
	return (resultat);
}

void afftab (int **tab, int x, int y){
	int i = 0;
	int j = 0;
	while(i<x){
		j=0;
		while(j<y){
			printf("%d\t", tab[i*y+j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

/* 
 * Entrées : choix du joueur, numéro du joueur, valeur du dé, tableau
 * à 2 dimensions contenant les positions des chevaux
 * Sortie : aucune
 * Action : met à jour les positions des chevaux en fonction du choix
 */
void appliquerChoix(int choix, int numJoueur, int valeurDe, int **positionChevaux){
	int **newPositionChevaux = positionChevaux;
	switch(choix){
		/* Sortir cheval */
		case 0 :
			newPositionChevaux[numJoueur][1] = getPremiereCaseJoueur(numJoueur);
			break;
		case 1 :
			newPositionChevaux[numJoueur][2] = getPremiereCaseJoueur(numJoueur);
			break;
		case 2 :
			newPositionChevaux[numJoueur][3] = getPremiereCaseJoueur(numJoueur);
			break;
		case 3 :
			newPositionChevaux[numJoueur][4] = getPremiereCaseJoueur(numJoueur);
			break;
		/* Avancer cheval */
		case 4 :
			newPositionChevaux[numJoueur][1] = getCaseSuivante(positionChevaux[numJoueur][1],valeurDe);
			break;
		case 5 :
			newPositionChevaux[numJoueur][2] = getCaseSuivante(positionChevaux[numJoueur][2],valeurDe);
			break;
		case 6 :
			newPositionChevaux[numJoueur][3] = getCaseSuivante(positionChevaux[numJoueur][3],valeurDe);
			break;
		case 7 :
			newPositionChevaux[numJoueur][4] = getCaseSuivante(positionChevaux[numJoueur][4],valeurDe);
			break;
		/* Monter escalier */
		case 8 :
			newPositionChevaux[numJoueur][1] = getmonterEscalier(positionChevaux[numJoueur][1],numJoueur);
			break;
		case 9 :
			newPositionChevaux[numJoueur][2] = getmonterEscalier(positionChevaux[numJoueur][2],numJoueur);
			break;
		case 10 :
			newPositionChevaux[numJoueur][3] = getmonterEscalier(positionChevaux[numJoueur][3],numJoueur);
			break;
		case 11 :
			newPositionChevaux[numJoueur][4] = getmonterEscalier(positionChevaux[numJoueur][4],numJoueur);
			break;
		/* Gagner cheval */
		case 12 :
			newPositionChevaux[numJoueur][1] = ParadisChevaux;
			break;
		case 13 :
			newPositionChevaux[numJoueur][2] = ParadisChevaux;
			break;
		case 14 :
			newPositionChevaux[numJoueur][3] = ParadisChevaux;
			break;
		case 15 :
			newPositionChevaux[numJoueur][4] = ParadisChevaux;
			break;
	}
}

/* 
 * Entrée : numéro du joueur
 * Sortie : numéro de la première case d'un joueur
 */
int getPremiereCaseJoueur(int numJoueur){
	switch(numJoueur){
		case 1 :
			return 1;
			break;
		case 2 :
			return 15;
			break;
		case 3 :
			return 29;
			break;
		case 4 :
			return 43;
			break;
		default :
			return 0;
	}
}

/* 
 * Entrées : case courante, valeur du dé
 * Sortie : numéro de la case suivante
 */
int getCaseSuivante(int caseCourante, int valeurDe){
	int futurCase = caseCourante + valeurDe;
	if(futurCase >= 57){
		futurCase -= 56;
	}
	return futurCase;
}

/* 
 * Entrées : case courante, numéro du joueur
 * Sortie : numéro de la marche d'escalier suivante en gérant la première
 */
int getmonterEscalier(int caseCourante, int numJoueur){
	if(caseCourante <= 56){
		return getPremierEscalierJoueur(numJoueur);
	}
	else{
		return caseCourante + 1;
	}
}

/* 
 * Entrées : numéro du joueur
 * Sortie : numéro de la première marche d'escalier d'un joueur
 */
int getPremierEscalierJoueur(int numJoueur){
	switch(numJoueur){
		case 1 :
			return 61;
			break;
		case 2 :
			return 71;
			break;
		case 3 :
			return 81;
			break;
		case 4 :
			return 91;
			break;
		default :
			return 0;
	}
}
