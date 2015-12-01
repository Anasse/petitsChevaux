#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "Reseau.h"
#include <sys/socket.h>

#define portPublic 12133

int inscription(int tubeOut, int tubeIn , int paramNbJoueurs);
int interface(int tubeOut, int tubeIn, int id);

int stringToInt (char* s){
	int res =0;
	int i=0;
	while(i < strlen(s)){
		res = 10*res + (s[i]-'0');
		i++;
	}
	return res;
}

int main (int nbArgs, char* args[]){
	if(nbArgs != 3){
		//nbArgs incorrect !
		printf("Vous devez fournir deux arguments,\nLe premier est le nombre de joueurs, le second le nombre de chevaux par joueur\n");
		return -1;
	}else{
		//Comportement Normal
		int paramNbJoueurs = stringToInt(args[1]);
		int paramNbChevaux = stringToInt(args[2]);
		int nbInscrits=0;
		int i =0;
		printf("Serveur des petits chevaux\nParties de %d joueurs à %d chevaux.\n", paramNbJoueurs, paramNbChevaux);
		pid_t p=getpid();
		
		int tube_inscription[2];//Création du tube Base-Inscription
		pipe(tube_inscription);

		int tubes_interfaces[paramNbJoueurs][2];//Création des tubes Base-interface
		i=0;
		while(i<paramNbJoueurs){
			pipe(tubes_interfaces[i]);
			i++;
		}

		p = fork(); //Création du processus inscription
		if(p==0){inscription(tube_inscription[0], tube_inscription[1], paramNbJoueurs); return 0;} //Tuer le processus fils

		int messageTubeInscription;
		while(nbInscrits < paramNbJoueurs){
			read(tube_inscription[0], &messageTubeInscription, sizeof(int));
			p = fork();
			if(p==0){interface(tubes_interfaces[nbInscrits][0], tubes_interfaces[nbInscrits][1], nbInscrits); return 0;}
			nbInscrits++;
		}
		 
		return 0;
	}
}

int inscription(int tubeIn, int tubeOut , int paramNbJoueurs){
	/*TRACE*/printf("Atteint Inscription !\n");
	int nbInscrits = 0;
	int UN = 1;
	int portPriveJoueur;
	int numServer = socketServer(portPublic,TCP);
	while(nbInscrits < paramNbJoueurs){
		printf("En attente d'une inscription...\n");
		int msgSock = accept(numServer, NULL, NULL);
		write(tubeOut, &UN, sizeof(int));
		portPriveJoueur = portPublic+nbInscrits+1;
		write(msgSock, &portPriveJoueur, sizeof(int));
		nbInscrits++;
		/*TRACE*/printf("Inscription : nbinscrits %d\n", nbInscrits);
	}
	portPriveJoueur = 0; //On n'accepte plus de nouvelles connexions
	while(1){
		int msgSock = accept(numServer, NULL, NULL);
		write(msgSock, &portPriveJoueur, sizeof(int));
	}
}

int interface(int tubeOut, int tubeIn, int idParam){
	/*TRACE*/printf("Atteint interface %d\n", idParam);
	int numJoueur = idParam+1;
	int numServer = socketServer(portPublic+numJoueur,TCP);
	int msgSock = accept(numServer, NULL, NULL);
	write(msgSock, &numJoueur, sizeof(int));

	int message;
	read(msgSock, &message, sizeof(int));
	printf("Message reçu : %d\n", message);
	write(msgSock, &message, sizeof(int));

	shutdown(msgSock, SHUT_RDWR);
	return 0;
}

