/* ---------------------------------------------------------------------------
 * Test de l'affichage du plateau pour le jeu des petits chevaux en mode texte
 * Auteur     : Damien Genthial
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
char whichChar(int pos) {
    // Pour faire un essai d'affichage de case non vide :
    if (pos == 44) return ROUGE;
    if (pos == 64) return ROUGE;

    if (pos <= 56) {
        if ((pos % 14) == 1)
            return '=';
        else return '.';
    } else {
        return '0' + (pos % 10);
    }
}

// Nombre de chevaux restants à l'écurie pour une équipe donnée
int nbHorsesHome(Team t) {
    return 2;
}

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

void appliquerChoix(int choix, int numJoueur, int nbCasesAAvancer, int **positionChevaux){
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
			newPositionChevaux[numJoueur][1] = getCaseSuivante(positionChevaux[numJoueur][1],nbCasesAAvancer);
			break;
		case 5 :
			newPositionChevaux[numJoueur][2] = getCaseSuivante(positionChevaux[numJoueur][2],nbCasesAAvancer);
			break;
		case 6 :
			newPositionChevaux[numJoueur][3] = getCaseSuivante(positionChevaux[numJoueur][3],nbCasesAAvancer);
			break;
		case 7 :
			newPositionChevaux[numJoueur][4] = getCaseSuivante(positionChevaux[numJoueur][4],nbCasesAAvancer);
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

int getCaseSuivante(int caseCourante, int nbCasesAAvancer){
	int futurCase = caseCourante + nbCasesAAvancer;
	if(futurCase >= 57){
		futurCase -= 56;
	}
	return futurCase;
}

int getmonterEscalier(int caseCourante, int numJoueur){
	if(caseCourante <= 56){
		return getPremierEscalierJoueur(numJoueur);
	}
	else{
		return caseCourante + 1;
	}
}

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
