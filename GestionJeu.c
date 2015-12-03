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
char whichChar(int pos, int nbC, int nbJ, int *posCh) {
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
		case 0 :
			return ROUGE;
			break;
		case 1 :
			return MAGENTA;
			break;
		case 2 :
			return VERTE;
			break;
		case 3 :
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
int nbHorsesHome(Team t, int nbC, int *posCh) {
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

void afftab (int *tab, int x, int y){
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
void appliquerChoix(int choix, int numJoueur, int valeurDe, int *positionChevaux, int nbC){
	int *newPositionChevaux = positionChevaux;
	switch(choix){
		/* Sortir cheval */
		case 0:
		case 1:
		case 2:
		case 3:
			newPositionChevaux[numJoueur*nbC+choix] = getPremiereCaseJoueur(numJoueur);
			//?Manger ?
			break;
		/* Avancer cheval */
		case 4:
		case 5:
		case 6:
		case 7:
			newPositionChevaux[numJoueur*nbC+(choix-4)] = getCaseSuivante(positionChevaux[numJoueur*nbC+(choix-4)],valeurDe);
			//?Manger?
			break;
		/* Monter escalier */
		case 8:
		case 9:
		case 10:
		case 11:
			newPositionChevaux[numJoueur*nbC+(choix-8)] = getmonterEscalier(positionChevaux[numJoueur*nbC+(choix-8)],numJoueur);
			break;
		/* Gagner cheval */
		case 12:
		case 13:
		case 14:
		case 15:
			newPositionChevaux[numJoueur*nbC+(choix-12)] = ParadisChevaux;
			break;
	}
}

/* 
 * Entrée : numéro du joueur
 * Sortie : numéro de la première case d'un joueur
 */
int getPremiereCaseJoueur(int numJoueur){
	switch(numJoueur){
		case 0 :
			return 1;
			break;
		case 1 :
			return 15;
			break;
		case 2 :
			return 29;
			break;
		case 3 :
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
		case 0 :
			return 61;
			break;
		case 1 :
			return 71;
			break;
		case 2 :
			return 81;
			break;
		case 3 :
			return 91;
			break;
		default :
			return 0;
	}
}

//Renvoie 1 si le joueur numJoueur à gagné, 0 sinon; d'après la position de ses chevaux dans posC
int aGagne (int numJoueur, int nbC ,int* posC){
	int i=0;
	while(i<nbC && posC[nbC*numJoueur+i] != ParadisChevaux){
		i++;
	}
	if(i==nbC){return 1;}
	else{return 0;}
}

void etendreVecteurChoix (char* old, char* new, int nbC){
	int j=0;
	int i=0;
	while( i <16){ //INIT
		new[i] = '0';
		i++;
	}
	i=0;
	while( j < 4){ //COPIE
		i=0;
		while(i<nbC){
			new[4*j+i] = old[j*nbC+i];
			i++;
		}
		j++;
	}
}
