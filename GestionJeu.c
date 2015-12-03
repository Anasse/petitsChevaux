/* ---------------------------------------------------------------------------
 * Test de l'affichage du plateau pour le jeu des petits chevaux en mode texte
 * Auteur     : Damien Genthial
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>  	
#define GESTION_JEU_C
#include "GestionJeu.h"

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
int nbHorsesHome(Team t){
    return 2;
}

int lancerDes(){
	srand(time(NULL));
	int resultat = (rand()%6 + 1) ;
	return (resultat);
}

void choix(int nChev, int nbreJoueur, int joueur, int *position, int des, char* c){
	
	
	init(c, 4*nChev);
	int positionFuture = 0;
	int positionPresent = 0;
	int ecurie = (joueur+1)*10 + 57 ;
	int i = 0;
	int j = 0 ;
	int sortie = 0 ; 
	switch(des){
		case 6 :
			while(i < nChev ){
				positionPresent = position[joueur*nChev + i];
				positionFuture = positionPresent+ des;
				/*TRACE*/ printf("Cheval position = %d \n", positionPresent);
				/*
				 * On verifie les ecuries d'abord si aucun cheval ne sort ou si l'ecurie 
				 * */
				if(position[joueur*nChev + i] == ecurie + i ){
					c[i]= '1';
				}
				if(positionPresent  >= ecurie && sortirCheval(position, nChev, joueur) ){

					c[i] = '1';
					sortie = 1 ; 
				}
				/*
				 * On commencer par verifier si la case d'arrive est differente de notre cheval de depat( meme race)
				 * Puis on verifie si le chemin est vide de chevaux :) 
				 * */
				 j=0;
			while( j < nChev){

				if(position[joueur*nChev +j] == positionFuture  ){
					j =20;
				}
				j++;
			}
			if(j == nChev && posAvancer(positionPresent, position, nChev, nbreJoueur, des) ){
					//on verifie si on atteint le debut des escaliers ou pas 
						switch(joueur){
							case 0 : 
								if(positionFuture <= 56 ){
										c[i + nChev]= '1';
									}
								break;
							default : 

								if(positionPresent <= joueur*14 && positionFuture > joueur*14){//dans le cas ou il y a un depassement 
										
										c[i + nChev]= '0';			
									}
								else if((positionPresent != joueur*10 + 50 +des - 1) && (positionPresent != (joueur+1)*10 + 56) &&(positionPresent != (joueur+1)*10 + 55) && (sortie == 0) && positionPresent < 67){
										c[i + nChev]= '1';
									}
								break;
							}
					}
					
				/*
				 * Grimper sur les escalier plus le cas de gagner
				 * */
				if(positionPresent == (joueur+1)*10 + 55){ 
					c[i + 2*nChev] = '1';
				}		
				if(positionPresent == (joueur+1)*10 + 56){ 
					c[i + 3*nChev] = '1';
				}	
				i++;
				sortie =0 ;
			}
			break;
			/*
			 * CAS OU LE DES != 6
			 * */
		default : 
			 i = 0 ;
			
			while(i < nChev){				
			positionPresent = position[joueur*nChev + i];
			positionFuture = positionPresent + des;
				/*
				 * On commencer par verifier si la case d'arrive est differente de notre cheval de depat( meme race)
				 * Puis on verifie si le chemin est vide de chevaux :) 
				 * */
			j = 0;
			while( j < nChev){

				
				if(position[joueur*nChev +j] == positionFuture   ){
					j =20;
				}
				j++;
			}
			
			
			if(j == nChev && (posAvancer(positionPresent, position, nChev, nbreJoueur, des))){
					//on verifie si on atteint le debut des escaliers ou pas 
						switch(joueur){
							case 0 : 
								if(positionFuture <= 56 ){
										c[i + nChev]= '1';
									}
								break;
							default : 
							
								if(positionPresent <= joueur*14 && positionFuture > joueur*14){//dans le cas ou il y a un depassement 
										c[i + nChev]= '0';			
									}
									
							else if((positionPresent != (joueur+1)*10 + 50 +des - 1)  && positionPresent <= 56 )
							{
										c[i + nChev]= '1';
									}
								break;
							}
					}
				if(joueur == 0 && des == 1 && positionPresent == 56){
						c[i + 2*nChev] = '1';
					}
				else if(des == 1 && positionPresent == joueur * 14 && joueur != 0){
						c[i +2*nChev] = '1';
					}
				else if(positionPresent == (joueur+1) * 10 + 50 + des -1){
					c[i + 2 *nChev]= '1';

					}
			
			i++;
		}
		break;
	}
	
}

	
	
int sortirCheval(int *position, int nbreChev, int joueur){
	int i = 0; 
	int retour = 1;
	while(i < nbreChev){
		/*TRACE*/ printf(" position = > \%d n", position[joueur*nbreChev + i]);
		/*TRACE*/ printf(" joueur*14 + 1 => %d\n ", joueur*14+1);

			if(position[joueur*nbreChev + i] == (joueur*14 + 1) ){
				retour  = 0; 
			}
			i++;
		}
		return retour;
}
/*
 * Fonction qui retourne vrai personne sur le chemin
 * */
int posAvancer(int actuel, int *positions, int nbreChev, int nbreJoueur, int des){
	int futur = actuel + des;

	int i = 0 ;
	int j = 0 ;
	int avancer =1 ;
	while(i < nbreJoueur){
		j =0;
		while(j < nbreChev && positions[i*nbreChev+j]){
			if(positions[i*nbreChev+j] < futur && positions[i*nbreChev+j]>actuel){
				avancer = 0;
				}
				j++;
		}
		i++;
	}
	return avancer ;
}


void afficher(int *pos, int x, int y){
	int i =0, j=0;
	while(i < x){
		j =0;
		while(j< y){ 
				printf("| %d ", pos[i*y +j]);
				j++;
			}
			printf("|\n");
			i++;
		}
	}
	
void afficherChoix(char *tab, int x){
	int i =0;
	while(i < x){
			printf("| %c \n", tab[i]);
			i++;
		
		}
	}

void init(char *tab, int x){
	int i =0;
	printf("INIITALISATION taille du tableau = %d \n", x);
	while(i < x){
			
			tab[i] = '0';
			//printf("| %c \n", tab[i]);
			i++;
		}
	}


// /*TRACE*/ printf(" \n");
