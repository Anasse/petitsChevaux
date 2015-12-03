/* ---------------------------------------------------------------------------
 * Test de l'affichage du plateau pour le jeu des petits chevaux en mode texte
 * Auteur     : Damien Genthial
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "Plateau.h"
#include "GestionJeu.h"

int main(int nbArgs, char* arg[])
{

	
	int des = lancerDes();
	printf(" Des = %d \n", des); 
	
	int nJoueur = 2;
	int nChev = 3;
	
	char c[4*nChev];
	
	int *position = malloc(sizeof(int)*nJoueur*nChev); 
	position[0] = 67; 
	position[1] =1;
	position[2] =101;

	position[1*nChev] = 77;
	position[1*nChev+1] = 79;
	position[1*nChev + 2] = 78;
	
	afficher(position, nJoueur,nChev);
	printf("\n");
	determinerChoix(nChev, nJoueur, 0, position, 6, c);
	afficherChoix(c, 4*nChev);
	
	/*  PRINCIPALE 
    // Pour un centrage dans une fenêtre 80x24
    //affichePlateau(5, 24);

    // Pour l'exemple, si on a besoin de parcourir la liste des équipes ?
    Team t;
    printf("Équipes :\n");
    // N'affiche rien car 'R' > 'B'
    for (t = ROUGE; t <= BLEUE; t++) {
        printf("\t%s\n", toString(t));
    }

    char equipes[] = {ROUGE, MAGENTA, VERTE, BLEUE};
    int i;
    printf("Équipes :\n");
    for (i = 0; i < 4; i++) {
        printf("\t%s\n", toString(equipes[i]));
    }*/
    return EXIT_SUCCESS;
}

