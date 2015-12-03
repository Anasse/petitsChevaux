#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "Plateau.h"
#include "GestionJeu.h"
#include "Reseau.h"
#include <sys/types.h>
#include <sys/socket.h>

void enumererChoixPossibles(int numSocket, int nbChevaux, int valeurDes, char* choix);

int main(int nbArgs, char* arg[]){
	
	/* Si il n'y a pas 2 arguments à la fonction on donne une aide */
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
		
	/* On lit notre numéro de joueur */
	read(numSocket, &numJoueur, sizeof(int));
	printf("Mon numéro de joueur est %d\n", numJoueur);
	
	/* On lit le nombre de joueurs */
	read(numSocket, &nbJoueurs, sizeof(int));
	printf("Il y a %d joueurs\n", nbJoueurs);
	
	/* On lit le nombre de chevaux par joueur */
	read(numSocket, &nbChevaux, sizeof(int));
	printf("Il y a %d chevaux par joueur\n", nbChevaux);
		
	/* On attend le signal de depart */
	int signal;
	read(numSocket, &signal, sizeof(int));
	printf("C'est parti ! (signal : %d)\n", signal);

	/*Début du jeu; on lit différentes informations*/
	int tour;
	int joueurDuTour;
	int *pos = malloc(sizeof(int)*nbJoueurs*nbChevaux);
	char *choix = malloc(4*nbChevaux*sizeof(char));
	int selectionChoix;
	
	/* Signal de départ */
	read(numSocket, &signal, sizeof(int));
	
	while(signal != 0){
		/* On lit le tour, le joueur du tour, la valeur du dé, et les 
		 * données de positionnement des chevaux */
		read(numSocket, &tour, sizeof(int));
		read(numSocket, &joueurDuTour, sizeof(int));
		read(numSocket, &valeurDe, sizeof(int));
		read(numSocket, pos, sizeof(int)*nbJoueurs*nbChevaux);
		/*TRACE-DEBUG*/afftab(pos, nbJoueurs, nbChevaux);
		/* On affiche le plateau de jeu */			
		affichePlateau(5, 25, nbJoueurs, nbChevaux, pos);
		printf("***Tour %d --- C'est au joueur %d !\tRésultat du dé : %d\n", tour, joueurDuTour+1, valeurDe);
			
		if(joueurDuTour == numJoueur-1){
			//printf("C'est mon tour !\n");
			// Enumeration des choix possibles
			read(numSocket,choix,4*nbChevaux*sizeof(char));
			enumererChoixPossibles(numSocket, nbChevaux, valeurDe, choix);
			// Lecture du choix du joueur 
			do{
				printf("--> Votre choix ? : ");
				scanf("%d", &selectionChoix);
			}while(choix[selectionChoix] != '1');
			// Envoi du choix du joueur au serveur 
			write(numSocket, &selectionChoix, sizeof(int));
		}else{
			printf("Tour des autres joueurs, merci de patienter.\n");
		}
		/* Lecture du signal pour savoir si le jeu est fini */
		read(numSocket, &signal, sizeof(int));
		printf("Je lit le signal %d\n", signal);
	}
	printf("je ferme\n");
	/* On ferme le socket */
	close(numSocket);
	return EXIT_SUCCESS;
}

/* 
 * Entrées : numéro de socket, nombre de chevaux par joueur, valeur du dé
 * Sortie : aucune
 * Action : affiche les choix possibles du joueur pour la suite du jeu
 */
void enumererChoixPossibles(int numSocket, int nbChevaux, int valeurDe, char* choix){
	//char choix[4*nbChevaux];
	//read(numSocket,choix,4*nbChevaux*sizeof(char));
	int auMoinsUnChoix = 0;
	int i = 0;
	while(i <= 7){
		if(choix[i] == '1'){
			auMoinsUnChoix = 1;
		}
		i++;
	}
	if(auMoinsUnChoix == 1){
		printf("Veuillez choisir un mouvement : \n");
		if(nbChevaux == 2){
			if(choix[0] == '1'){
				printf("0 - Sortir un cheval 1\n");
			}
			if(choix[1] == '1'){
				printf("1 - Sortir un cheval 2\n");
			}
			if(choix[2] == '1'){
				printf("4 - Avancer le cheval 1 de %d case(s)\n", valeurDe);
			}
			if(choix[3] == '1'){
				printf("5 - Avancer le cheval 2 de %d case(s)\n", valeurDe);
			}
			if(choix[4] == '1'){
				printf("8 - Monter l'escalier avec le cheval 1\n");
			}
			if(choix[5] == '1'){
				printf("9 - Monter l'escalier avec le cheval 2\n");
			}
			if(choix[6] == '1'){
				printf("12 - Faire gagner le cheval 1\n");
			}
			if(choix[7] == '1'){
				printf("13 - Faire gagner le cheval 2\n");
			}
		}
		else if(nbChevaux == 3){
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
				printf("4 - Avancer le cheval 1 de %d case(s)\n", valeurDe);
			}
			if(choix[4] == '1'){
				printf("5 - Avancer le cheval 2 de %d case(s)\n", valeurDe);
			}
			if(choix[5] == '1'){
				printf("6 - Avancer le cheval 3 de %d case(s)\n", valeurDe);
			}
			if(choix[6] == '1'){
				printf("8 - Monter l'escalier avec le cheval 1\n");
			}
			if(choix[7] == '1'){
				printf("9 - Monter l'escalier avec le cheval 2\n");
			}
			if(choix[8] == '1'){
				printf("10 - Monter l'escalier avec le cheval 3\n");
			}
			if(choix[9] == '1'){
				printf("12 - Faire gagner le cheval 1\n");
			}
			if(choix[10] == '1'){
				printf("13 - Faire gagner le cheval 2\n");
			}
			if(choix[11] == '1'){
				printf("14 - Faire gagner le cheval 3\n");
			}
		}
		else if(nbChevaux == 4){
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
	else{
		printf("Aucun mouvement possible\n");
	}
}

