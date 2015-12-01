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

int main(int nbArgs, char* arg[]){
	
	int numPortPrive, numJoueur;
	
	int arg2 = atoi(arg[2]);	
	int numSocket = socketClient(arg[1], arg2, TCP);
	if(numSocket == -1){
		fprintf(stderr, "Erreur de création du socket !\n");
		return EXIT_FAILURE;
	}
	
	printf("Connexion au socket public réussie\n");							//DEBUG
	printf("J'attend mon numero de port\n");								//DEBUG
	
	read(numSocket, &numPortPrive, sizeof(int));
	if(numPortPrive == 0){
		printf("La partie est pleine ou déjà en cours !\n");
		close(numSocket);
		return EXIT_SUCCESS;
	}
	
	printf("Je dois me connecter au port %d\n", numPortPrive);
	
	close(numSocket);
	printf("Socket public fermé\n");										//DEBUG
	
	numSocket = socketClient(arg[1], numPortPrive, TCP);
	if(numSocket == -1){
		fprintf(stderr, "Erreur de création du socket !\n");
		return EXIT_FAILURE;
	}
	
	printf("Connexion au socket privé réussie\n");							//DEBUG
	
	read(numSocket, &numJoueur, sizeof(int));
	printf("Mon numéro de joueur est %d\n", numJoueur);
	
	int signalDepart;
	read(numSocket, &signalDepart, sizeof(int));
	printf("signal : %d\n", signalDepart);
	
	return EXIT_SUCCESS;
}
