#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "Plateau.h"
#include "GestionJeu.h"
#include "Reseau.h"
#include <sys/types.h>
#include <sys/socket.h>

void enumererChoixPossibles(int numSocket, int nbChevaux, int valeurDes, char* choix, int auMoinsUnChoix, char* nomCh);

int main(int nbArgs, char* arg[]){
	
	/* Si il n'y a pas 2 arguments à la fonction on donne une aide */
	if(nbArgs != 3){
		printf("Veuillez entrer 2 paramètres : - le nom du serveur\n - le numéro de port\n");
		return EXIT_FAILURE;
	}
	
	int numJoueur, nbJoueurs, nbChevaux, valeurDe;
	char* couleurJoueur = malloc(10*sizeof(char));
	int arg2 = atoi(arg[2]);
	
	/* On se connecte au socket public */
	int numSocket = socketClient(arg[1], arg2, TCP);
	if(numSocket == -1){
		perror("Erreur de création du socket !\n");
		return EXIT_FAILURE;
	}
		
	/* On lit notre numéro de joueur */
	read(numSocket, &numJoueur, sizeof(int));
	switch(numJoueur){
			case 0 :
				couleurJoueur = "Rouge\0";
				break;
			case 1 :
				couleurJoueur = "Magenta\0";
				break;
			case 2 :
				couleurJoueur = "Vert\0";
				break;
			default :
				couleurJoueur = "Bleu\0";
		}
	printf("Ma couleur est %s\n", couleurJoueur);
	//printf("Mon numéro de joueur est %d\n", numJoueur);
	
	/* On lit le nombre de joueurs */
	read(numSocket, &nbJoueurs, sizeof(int));
	printf("Il y a %d joueurs\n", nbJoueurs);
	
	/* On lit le nombre de chevaux par joueur */
	read(numSocket, &nbChevaux, sizeof(int));
	printf("Il y a %d chevaux par joueur\n", nbChevaux);
	
	//Capture du nom des chevaux
	int i = 0;
	char* nomChevauxJ = malloc(nbChevaux*sizeof(char));
	char* buffer = malloc(100);
	char* nomChevauxTous = malloc(nbChevaux*nbJoueurs*sizeof(char));
	while(i<nbChevaux){
		printf("Saissisez un caractère pour symboliser votre cheval %d : ", i);
		scanf("%s", buffer);
		nomChevauxJ[i] = buffer[0];
		i++;
	}
	write(numSocket, nomChevauxJ, nbChevaux*sizeof(char));
	printf("En attente des autres joueurs...\n");
	read(numSocket, nomChevauxTous, nbChevaux*nbJoueurs*sizeof(char));
	/* On attend le signal de depart */
	int signal;
	read(numSocket, &signal, sizeof(int));
	printf("Tous le monde est connecté, c'est parti !\n");

	/*Début du jeu; on lit différentes informations*/
	int tour;
	int joueurDuTour;
	char* couleurJoueurDuTour = malloc(10*sizeof(char));
	int *pos = malloc(sizeof(int)*nbJoueurs*nbChevaux);
	char *choix = malloc(4*nbChevaux*sizeof(char));
	int selectionChoix;
	int auMoinsUnChoix = 0;
	int *ptrC = malloc(sizeof(int));//Utilisé dans affichage plateau, déclaré ici pour éviter les trop nombreux mallocs
	i = 0;
	int vainqueur=666;
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
		affichePlateau(2, 22, nbJoueurs, nbChevaux, pos, nomChevauxTous, ptrC);
		switch(joueurDuTour){
			case 0 :
				couleurJoueurDuTour = "Rouge\0";
				break;
			case 1 :
				couleurJoueurDuTour = "Magenta\0";
				break;
			case 2 :
				couleurJoueurDuTour = "Vert\0";
				break;
			default :
				couleurJoueurDuTour = "Bleu\0";
		}
		printf("***Tour %d --- C'est au joueur %s !\tRésultat du dé : %d\n", tour, couleurJoueurDuTour, valeurDe);
		/*
		printf("____\n|\ %d \\\n| \___\\\n\ |   |\n\|___|\n", valeurDe);
			 ____
			|\ 6 \
			| \___\
			\ |   |
			 \|___|
		
		 ---
		| 6 |
		 ---
		*/
		if(joueurDuTour == numJoueur-1){
			//printf("C'est mon tour !\n");
			// Enumeration des choix possibles
			read(numSocket,choix,16*sizeof(char));
			i = 0;
			auMoinsUnChoix = 0;
			while(i < 16 && choix[i] == '0'){
				i++;
			}
			if(i<16){auMoinsUnChoix = 1;}else{auMoinsUnChoix = 0;}
			enumererChoixPossibles(numSocket, nbChevaux, valeurDe, choix, auMoinsUnChoix, nomChevauxJ);
			//afficherChoix(choix, 16);
			// Lecture du choix du joueur 
			do{
				printf("--> Votre choix ? : ");
				scanf("%d", &selectionChoix);
			}while((auMoinsUnChoix == 1 &&(selectionChoix < 0 || selectionChoix > 16 || choix[selectionChoix] == '0'))||
			(auMoinsUnChoix == 0 && selectionChoix != 20));
			// Envoi du choix du joueur au serveur 
			write(numSocket, &selectionChoix, sizeof(int));
		}else{
			printf("Tour des autres joueurs, merci de patienter...\n");
		}
		/* Lecture du signal pour savoir si le jeu est fini */
		read(numSocket, &signal, sizeof(int));
	}
	read(numSocket, &vainqueur, sizeof(int));
	printf("*************************************************\n");
	printf("Fin de de la partie, acclamons le vainqueur, le joueur %d\n\n", vainqueur+1);
	/* On ferme le socket */
	close(numSocket);
	return EXIT_SUCCESS;
}

/* 
 * Entrées : numéro de socket, nombre de chevaux par joueur, valeur du dé
 * Sortie : aucune
 * Action : affiche les choix possibles du joueur pour la suite du jeu
 */
void enumererChoixPossibles(int numSocket, int nbChevaux, int valeurDe, char* choix, int auMoinsUnChoix, char* nomsCh){
	//char choix[4*nbChevaux];
	//read(numSocket,choix,4*nbChevaux*sizeof(char));
	if(auMoinsUnChoix == 1){
		printf("Veuillez choisir un mouvement : \n");
		if(choix[0] == '1'){
			printf("0 - Sortir le cheval %c\n", nomsCh[0]);
		}
		if(choix[1] == '1'){
			printf("1 - Sortir le cheval %c\n", nomsCh[1]);
		}
		if(choix[2] == '1'){
			printf("2 - Sortir le cheval %c\n", nomsCh[2]);
		}
		if(choix[3] == '1'){
			printf("3 - Sortir le cheval %c\n", nomsCh[3]);
		}
		if(choix[4] == '1'){
			printf("4 - Avancer le cheval %c de %d case(s)\n", nomsCh[0], valeurDe);
		}
		if(choix[5] == '1'){
			printf("5 - Avancer le cheval %c de %d case(s)\n", nomsCh[1], valeurDe);
		}
		if(choix[6] == '1'){
			printf("6 - Avancer le cheval %c de %d case(s)\n", nomsCh[2], valeurDe);
		}
		if(choix[7] == '1'){
			printf("7 - Avancer le cheval %c de %d case(s)\n", nomsCh[3], valeurDe);
		}
		if(choix[8] == '1'){
			printf("8 - Monter l'escalier avec le cheval %c\n", nomsCh[0]);
		}
		if(choix[9] == '1'){
			printf("9 - Monter l'escalier avec le cheval %c\n", nomsCh[1]);
		}
		if(choix[10] == '1'){
			printf("10 - Monter l'escalier avec le cheval %c\n", nomsCh[2]);
		}
		if(choix[11] == '1'){
			printf("11 - Monter l'escalier avec le cheval %c\n", nomsCh[3]);
		}
		if(choix[12] == '1'){
			printf("12 - Faire gagner le cheval %c\n", nomsCh[0]);
		}
		if(choix[13] == '1'){
			printf("13 - Faire gagner le cheval %c\n", nomsCh[1]);
		}
		if(choix[14] == '1'){
			printf("14 - Faire gagner le cheval %c\n", nomsCh[2]);
		}
		if(choix[15] == '1'){
			printf("15 - Faire gagner le cheval %c\n", nomsCh[3]);
		}
	}
	else{
		printf("Aucun mouvement possible, saisir '20' pour confirmer.\n");
	}
}

