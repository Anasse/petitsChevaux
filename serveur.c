#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "Reseau.h"
#include <sys/socket.h>

#define portPublic 17499

int inscription(int tubeOut, int tubeIn , int paramNbJoueurs);
int interface(int tubeIn, int tubeOut, int idParam, int sock, int nbJoueurs, int nbChevaux);
int stringToInt (char* s);

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
			printf("%d joueurs inscrits sur %d\n.", nbInscrits, paramNbJoueurs);
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
		//int posChevaux[paramNbJoueurs][paramNbChevaux] = initChevaux(paramNbJoueurs, paramNbChevaux);
		
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

/*int[][] initChevaux (int paramNbJoueurs, int paramNbChevaux){
	int posChevaux[paramNbJoueurs][paramNbChevaux];
	int i = 0;
	int j = 0;
	while(i<paramNbJoueurs){
		j=0;
		int ecurie = 10*(i+1)+57;
		while(j<paramNbChevaux){
			posChevaux[i][j] = ecurie+j;
			j++;
		}
		i++;
	}
	return posChevaux;
}*/

