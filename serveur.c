#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "Reseau.h"
#include "GestionJeu.h"
#include <sys/socket.h>
#include <stdlib.h>


int inscription(int tubeOut, int tubeIn , int paramNbJoueurs);
int interface(int tubeIn, int tubeOut, int idParam, int sock, int nbJoueurs, int nbChevaux);
int stringToInt (char* s);
void initChevaux (int* posChevaux, int paramNbJoueurs, int paramNbChevaux);


int main (int nbArgs, char* args[]){
	if(nbArgs != 4){
		//nbArgs incorrect !
		printf("Vous devez fournir trois arguments :\n-Le premier est le nombre de joueurs,\n-le second le nombre de chevaux par joueur,\n-Le troisième est le port réseau utilisé.\n");
		return 1;
	}else{
		//Comportement Normal, nombre d'arguments correct.
		int paramNbJoueurs = stringToInt(args[1]);
		int paramNbChevaux = stringToInt(args[2]);
		int portPublic = stringToInt(args[3]);
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
				close(numServer);
				interface(tubes_interfaces[2*nbInscrits+1][0],tubes_interfaces[2*nbInscrits][1], nbInscrits, msgSock, paramNbJoueurs, 					paramNbChevaux);
				return 0;
			}
			close(msgSock);
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
		int *posChevaux = malloc(sizeof(int)*paramNbJoueurs*paramNbChevaux);
		initChevaux(posChevaux, paramNbJoueurs, paramNbChevaux);
		afftab(posChevaux, paramNbJoueurs, paramNbChevaux);
		int tour = 0;
		int joueurDuTour = 0;
		int selectionChoix;
		char *choixTemp = malloc(paramNbChevaux*4*sizeof(char));
		char *choix = malloc(16*sizeof(char));
		signal = 1;
		while(tour < 50){
			int de = lancerDes();
			//printf("Tour:%d  dé:%d  joueur:%d\n", tour, de, joueurDuTour);
			i=0;
			while(i<paramNbJoueurs){
				write(tubes_interfaces[2*i+1][1], &signal, sizeof(int));
				write(tubes_interfaces[2*i+1][1], &tour, sizeof(int));
				write(tubes_interfaces[2*i+1][1], &joueurDuTour, sizeof(int));
				write(tubes_interfaces[2*i+1][1], &de, sizeof(int));
				write(tubes_interfaces[2*i+1][1], posChevaux, sizeof(int)*paramNbChevaux*paramNbJoueurs);
				i++;
			}
			//APPEL possibilités
			determinerChoix(paramNbChevaux, paramNbJoueurs, joueurDuTour, posChevaux, de, choixTemp);
			etendreVecteurChoix(choixTemp, choix, paramNbChevaux); 
			write(tubes_interfaces[2*joueurDuTour+1][1], choix, sizeof(char)*16);
			read(tubes_interfaces[2*joueurDuTour][0], &selectionChoix, sizeof(int));
			printf("Choix du client : %d\n", selectionChoix);
			if(selectionChoix != 20){
				//??appliquer
				appliquerChoix(selectionChoix, joueurDuTour, de, posChevaux, paramNbChevaux);
			}
			if(de !=6){joueurDuTour = (joueurDuTour+1)%paramNbJoueurs;}
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
	int *pos = malloc(sizeof(int)*nbJoueurs*nbChevaux);
	char *choix = malloc(4*nbChevaux*sizeof(char));
	int selectionChoix;
	read(tubeIn, &signal, sizeof(int));
	while(signal !=0){
		read(tubeIn, &tour, sizeof(int));
		read(tubeIn, &joueurDuTour, sizeof(int));
		read(tubeIn, &de, sizeof(int));
		read(tubeIn, pos, sizeof(int)*nbJoueurs*nbChevaux);
		afftab(pos, nbJoueurs, nbChevaux);
		write(sock, &signal, sizeof(int));	
		write(sock, &tour, sizeof(int));
		write(sock, &joueurDuTour, sizeof(int));
		write(sock, &de, sizeof(int));
		write(sock, pos, sizeof(int)*nbJoueurs*nbChevaux);
		if(joueurDuTour == idParam){
			//C'est mon tour
			read(tubeIn, choix, 16*sizeof(char));
			write(sock, choix, 16*sizeof(char));
			read(sock, &selectionChoix, sizeof(int));
			write(tubeOut, &selectionChoix, sizeof(int));
		}
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

void initChevaux (int* posChevaux, int paramNbJoueurs, int paramNbChevaux){
	int i = 0;
	int j = 0;
	while(i<paramNbJoueurs){
		j=0;
		int ecurie = 10*(i+1)+57;
		while(j<paramNbChevaux){
			posChevaux[i*paramNbChevaux+j] = ecurie+j;
			j++;
		}
		i++;
	}
}

