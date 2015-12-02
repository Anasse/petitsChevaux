#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "Reseau.h"
#include "GestionJeu.h"
#include <sys/socket.h>
#include <stdlib.h>

#define portPublic 17499

int inscription(int tubeOut, int tubeIn , int paramNbJoueurs);
int interface(int tubeIn, int tubeOut, int idParam, int sock, int nbJoueurs, int nbChevaux);
int stringToInt (char* s);
int **initChevaux (int paramNbJoueurs, int paramNbChevaux);


int main (int nbArgs, char* args[]){
	if(nbArgs != 3){
		//nbArgs incorrect !
		printf("Vous devez fournir deux arguments,\nLe premier est le nombre de joueurs, le second le nombre de chevaux par joueur\n");
		return 1;
	}else{
		//Comportement Normal, nombre d'arguments correct.
		int paramNbJoueurs = stringToInt(args[1]);
		int paramNbChevaux = stringToInt(args[2]);
		int nbInscrits=0;
		int i = 0;
		printf("Serveur des petits chevaux\nParties de %d joueurs à %d chevaux.\n", paramNbJoueurs, paramNbChevaux);
		pid_t p=getpid();
		//Création des tubes Base-interface
		int tubes_interfaces[paramNbJoueurs*2][2];
		i=0;
		while(i<paramNbJoueurs*2){
			pipe(tubes_interfaces[i]);
			i++;
		}
		//
		int numServer = socketServer(portPublic,TCP);
		int msgSock;
		while(nbInscrits < paramNbJoueurs){
			printf("En attente d'une inscription...\n");
			msgSock = accept(numServer, NULL, NULL);
			p=fork();
			if(p==0){
				interface(tubes_interfaces[2*nbInscrits+1][0],tubes_interfaces[2*nbInscrits][1], nbInscrits, msgSock, paramNbJoueurs, 					paramNbChevaux);
				return 0;
			}
			nbInscrits++;
			printf("%d joueurs inscrits sur %d.\n", nbInscrits, paramNbJoueurs);
		}
		shutdown(numServer, SHUT_RDWR);
		//Envoi du signal de départ
		printf("Début de la partie !\n");
		i=0;
		int signal = 1;
		while(i<paramNbJoueurs){
			write(tubes_interfaces[2*i+1][1], &signal, sizeof(int));
			i++;
		}
		//JEU
		int **posChevaux = initChevaux(paramNbJoueurs, paramNbChevaux);
		afftab(posChevaux, paramNbJoueurs, paramNbChevaux);
		int tour = 0;
		int joueurDuTour = 0;
		signal = 1;
		posChevaux[0]=4;
		posChevaux[1]=93;
		posChevaux[2]=64;
		posChevaux[3]=21;
		afftab(posChevaux, paramNbJoueurs, paramNbChevaux);
		while(tour < 2){
			int de = lancerDes();
			printf("Tour:%d  dé:%d  joueur:%d\n", tour, de, joueurDuTour);
			i=0;
			while(i<paramNbJoueurs){
				write(tubes_interfaces[2*i+1][1], &signal, sizeof(int));
				write(tubes_interfaces[2*i+1][1], &tour, sizeof(int));
				write(tubes_interfaces[2*i+1][1], &joueurDuTour, sizeof(int));
				write(tubes_interfaces[2*i+1][1], &de, sizeof(int));
				write(tubes_interfaces[2*i+1][1], posChevaux, sizeof(int*)*paramNbChevaux*paramNbJoueurs);
				i++;
			}
			joueurDuTour = (joueurDuTour+1)%paramNbJoueurs;
			tour ++;
		}
		i=0;
		signal = 0;//Signal de fin de la partie;
		while(i<paramNbJoueurs){
			write(tubes_interfaces[2*i+1][1], &signal, sizeof(int));
			i++;
		}		
		return 0;
	}
}

int interface(int tubeIn, int tubeOut, int idParam, int sock, int nbJoueurs, int nbChevaux){
	int numJoueur = idParam+1;
	int nbJ = nbJoueurs;
	int nbC = nbChevaux;
	write(sock, &numJoueur, sizeof(int));
	write(sock, &nbJ, sizeof(int));	
	write(sock, &nbC, sizeof(int));

	int signal;
	read(tubeIn, &signal, sizeof(int));
	write(sock, &signal, sizeof(int));

	int tour;
	int joueurDuTour;
	int de;
	int **pos = malloc(sizeof(int)*nbJoueurs*nbChevaux);

	read(tubeIn, &signal, sizeof(int));
	while(signal !=0){
		read(tubeIn, &tour, sizeof(int));
		read(tubeIn, &joueurDuTour, sizeof(int));
		read(tubeIn, &de, sizeof(int));
		read(tubeIn, pos, sizeof(int*)*nbJoueurs*nbChevaux);
		write(sock, &signal, sizeof(int));	
		write(sock, &tour, sizeof(int));
		write(sock, &joueurDuTour, sizeof(int));
		write(sock, &de, sizeof(int));
		write(sock, pos, sizeof(int*)*nbJoueurs*nbChevaux);

		read(tubeIn, &signal, sizeof(int));
	}
	write(sock, &signal, sizeof(int));

	shutdown(sock, SHUT_RDWR);
	return 0;
}

int stringToInt (char* s){
	int res =0;
	int i=0;
	while(i < strlen(s)){
		res = 10*res + (s[i]-'0');
		i++;
	}
	return res;
}

int **initChevaux (int paramNbJoueurs, int paramNbChevaux){
	int **posChevaux = malloc(sizeof(int)*paramNbJoueurs*paramNbChevaux);
	int i = 0;
	int j = 0;
	while(i<paramNbJoueurs){
		j=0;
		int ecurie = 10*(i+1)+57;
		while(j<paramNbChevaux){
			printf("init : %d\n", ecurie+j);
			posChevaux[i*paramNbChevaux+j] = ecurie+j;
			j++;
		}
		i++;
	}
	return posChevaux;
}

