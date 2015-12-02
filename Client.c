#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "Plateau.h"
#include "GestionJeu.h"
#include "Reseau.h"
#include <sys/types.h>
#include <sys/socket.h>

#define MAX 256
#define publicPort 7777

void enumererChoixPossibles(int numSocket, int nbChevaux, int valeurDes);

int main(int nbArgs, char* arg[]){
	
	if(nbArgs != 3){
		printf("Veuillez entrer 2 paramètres : - le nom du serveur\n - le numéro de port\n");
		return EXIT_FAILURE;
	}
	
	int numJoueur, nbJoueurs, nbChevaux, valeurDe;
	int arg2 = atoi(arg[2]);
	
	/* On se connecte au socket public */
	int numSocket = socketClient(arg[1], arg2, TCP);
	if(numSocket == -1){
		perror("Erreur de création du socket !\n");
		return EXIT_FAILURE;
	}
		
	/* On lit notre numero de joueur */
	read(numSocket, &numJoueur, sizeof(int));
	printf("Mon numéro de joueur est %d\n", numJoueur);
	
	/* On lit le nombre de joueurs */
	read(numSocket, &nbJoueurs, sizeof(int));
	printf("Il y a %d joueurs\n", nbJoueurs);
	
	/* On lit le nombre de chevaux */
	read(numSocket, &nbChevaux, sizeof(int));
	printf("Il y a %d chevaux par joueur\n", nbChevaux);
	
	int signal;
		
	/* On attend et on lit le signal de depart */
	read(numSocket, &signal, sizeof(int));
	printf("C'est parti ! (signal : %d)\n", signal);

	/*Début du jeu; on lit différentes informations*/
	int tour;
	int joueurDuTour;
	int **pos = malloc(sizeof(int)*nbJoueurs*nbChevaux);
	char *choix = malloc(16*sizeof(char));
	int selectionChoix;
	read(numSocket, &signal, sizeof(int));
	while(signal != 0){
		read(numSocket, &tour, sizeof(int));
		read(numSocket, &joueurDuTour, sizeof(int));
		read(numSocket, &valeurDe, sizeof(int));
		read(numSocket, pos, sizeof(int*)*nbJoueurs*nbChevaux);
		//read(numSocket, pos, sizeof(int)*nbJoueurs*nbChevaux);
		printf("***Tour %d --- C'est au joueur %d !\tRésultat du dé : %d\n", tour, joueurDuTour+1, valeurDe);
		afftab(pos, nbJoueurs, nbChevaux);
		affichePlateau(5, 5, nbJoueurs, nbChevaux, pos);
		if(joueurDuTour == numJoueur-1){
			//printf("C'est mon tour !\n");
			enumererChoixPossibles(numSocket, nbChevaux, valeurDe);
			printf("--> Votre choix ? : \n");
			scanf("%d", &selectionChoix);
			//??Verif saisie
			write(numSocket, &selectionChoix, sizeof(int));
		}
		read(numSocket, &signal, sizeof(int));
	}
	
	
	//int jouer = 1;
	//while(jouer){
		///* On lit le num du joueur du tour */
		//int joueurDuTour;
		//read(numSocket, &joueurDuTour, sizeof(int));
		//printf("C'est le tour du joueur numéro %d\n", joueurDuTour);
		
		//if(joueurDuTour != numJoueur){
			//printf("C'est votre tour");
		//}
		
		///* On lit la valeur du de */
		//read(numSocket, &valeurDe, sizeof(int));
		//printf("La valeur du dé est : %d\n", valeurDe);
		
		////Lire les données du tour
				
		//if(joueurDuTour != numJoueur){
			//// Jouer son tour
			
			//enumererChoixPossibles(numSocket, nbChevaux, valeurDe);
			//scanf(%d
		//}
	//}
	
	close(numSocket);
	return EXIT_SUCCESS;
}

void enumererChoixPossibles(int numSocket, int nbChevaux, int valeurDe){
	printf("Veuillez choisir un mouvement : \n");
	if(nbChevaux == 2){
		char choix[8];
		read(numSocket,choix,8);
		if(choix[0] == '1'){
			printf("0 - Sortir un cheval 1\n");
		}
		if(choix[1] == '1'){
			printf("1 - Sortir un cheval 2\n");
		}
		if(choix[4] == '1'){
			printf("4 - Avancer le cheval 1 de %d case(s)\n", valeurDe);
		}
		if(choix[5] == '1'){
			printf("5 - Avancer le cheval 2 de %d case(s)\n", valeurDe);
		}
		if(choix[8] == '1'){
			printf("8 - Monter l'escalier avec le cheval 1\n");
		}
		if(choix[9] == '1'){
			printf("9 - Monter l'escalier avec le cheval 2\n");
		}
		if(choix[12] == '1'){
			printf("12 - Faire gagner le cheval 1\n");
		}
		if(choix[13] == '1'){
			printf("13 - Faire gagner le cheval 2\n");
		}
	}
	else if(nbChevaux == 3){
		char choix[12];
		read(numSocket,choix,12);
		if(choix[0] == '1'){
			printf("0 - Sortir un cheval 1\n");
		}
		if(choix[1] == '1'){
			printf("1 - Sortir un cheval 2\n");
		}
		if(choix[2] == '1'){
			printf("2 - Sortir un cheval 3\n");
		}
		if(choix[4] == '1'){
			printf("4 - Avancer le cheval 1 de %d case(s)\n", valeurDe);
		}
		if(choix[5] == '1'){
			printf("5 - Avancer le cheval 2 de %d case(s)\n", valeurDe);
		}
		if(choix[6] == '1'){
			printf("6 - Avancer le cheval 3 de %d case(s)\n", valeurDe);
		}
		if(choix[8] == '1'){
			printf("8 - Monter l'escalier avec le cheval 1\n");
		}
		if(choix[9] == '1'){
			printf("9 - Monter l'escalier avec le cheval 2\n");
		}
		if(choix[10] == '1'){
			printf("10 - Monter l'escalier avec le cheval 3\n");
		}
		if(choix[12] == '1'){
			printf("12 - Faire gagner le cheval 1\n");
		}
		if(choix[13] == '1'){
			printf("13 - Faire gagner le cheval 2\n");
		}
		if(choix[14] == '1'){
			printf("14 - Faire gagner le cheval 3\n");
		}
	}
	else if(nbChevaux == 4){
		char choix[16];
		read(numSocket,choix,16);
		if(choix[0] == '1'){
			printf("0 - Sortir un cheval 1\n");
		}
		if(choix[1] == '1'){
			printf("1 - Sortir un cheval 2\n");
		}
		if(choix[2] == '1'){
			printf("2 - Sortir un cheval 3\n");
		}
		if(choix[3] == '1'){
			printf("3 - Sortir un cheval 4\n");
		}
		if(choix[4] == '1'){
			printf("4 - Avancer le cheval 1 de %d case(s)\n", valeurDe);
		}
		if(choix[5] == '1'){
			printf("5 - Avancer le cheval 2 de %d case(s)\n", valeurDe);
		}
		if(choix[6] == '1'){
			printf("6 - Avancer le cheval 3 de %d case(s)\n", valeurDe);
		}
		if(choix[7] == '1'){
			printf("7 - Avancer le cheval 4 de %d case(s)\n", valeurDe);
		}
		if(choix[8] == '1'){
			printf("8 - Monter l'escalier avec le cheval 1\n");
		}
		if(choix[9] == '1'){
			printf("9 - Monter l'escalier avec le cheval 2\n");
		}
		if(choix[10] == '1'){
			printf("10 - Monter l'escalier avec le cheval 3\n");
		}
		if(choix[11] == '1'){
			printf("11 - Monter l'escalier avec le cheval 4\n");
		}
		if(choix[12] == '1'){
			printf("12 - Faire gagner le cheval 1\n");
		}
		if(choix[13] == '1'){
			printf("13 - Faire gagner le cheval 2\n");
		}
		if(choix[14] == '1'){
			printf("14 - Faire gagner le cheval 3\n");
		}
		if(choix[15] == '1'){
			printf("15 - Faire gagner le cheval 4\n");
		}
	}
}

