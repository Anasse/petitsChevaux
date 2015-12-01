#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "Reseau.h"
#include <sys/socket.h>

#define portPublic 17499

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
		//Comportement Normal, nombre d'arguments correct.
		int paramNbJoueurs = stringToInt(args[1]);
		int paramNbChevaux = stringToInt(args[2]);
		int nbInscrits=0;
		int i =0;
		printf("Serveur des petits chevaux\nParties de %d joueurs à %d chevaux.\n", paramNbJoueurs, paramNbChevaux);
		pid_t p=getpid();
		//Création du tube Base-Inscription
		int tube_inscription[2];
		pipe(tube_inscription);
		//Création des tubes Base-interface
		int tubes_interfaces[paramNbJoueurs][2];
		i=0;
		while(i<paramNbJoueurs){
			pipe(tubes_interfaces[i]);
			i++;
		}
		//Création du processus inscription
		p = fork(); 
		if(p==0){inscription(tube_inscription[0], tube_inscription[1], paramNbJoueurs); return 0;} //Tuer le processus fils
		//Création des interfaces privées pour chaque client
		int messageTubeInscription;
		while(nbInscrits < paramNbJoueurs){
			read(tube_inscription[0], &messageTubeInscription, sizeof(int));
			p = fork();
			if(p==0){interface(tubes_interfaces[nbInscrits][0], tubes_interfaces[nbInscrits][1], nbInscrits); return 0;}
			nbInscrits++;
		}
		//Envoi du signal de départ
		printf("Début de la partie !\n");
		i=0;
		int signal = 1;
		while(i<paramNbJoueurs){
			write(tubes_interfaces[i][1], &signal, sizeof(int));
			i++;
		}
		
		return 0;
	}
}

int inscription(int tubeIn, int tubeOut , int paramNbJoueurs){
	int nbInscrits = 0;
	int UN = 1;
	int portPriveJoueur;
	int numServer = socketServer(portPublic,TCP);
	int msgSock;
	while(nbInscrits < paramNbJoueurs){
		printf("En attente d'une inscription...\n");
		msgSock = accept(numServer, NULL, NULL);
		write(tubeOut, &UN, sizeof(int));
		portPriveJoueur = portPublic+nbInscrits+1;
		write(msgSock, &portPriveJoueur, sizeof(int));
		nbInscrits++;
		/*TRACE*/printf("Inscription : nbinscrits %d\n", nbInscrits);
	}
	/*portPriveJoueur = 0; //On n'accepte plus de nouvelles connexions
	while(1){
		int msgSock = accept(numServer, NULL, NULL);
		write(msgSock, &portPriveJoueur, sizeof(int));
	}*/
	shutdown(msgSock, SHUT_RDWR);
	return 0;
}

int interface(int tubeIn, int tubeOut, int idParam){
	int numJoueur = idParam+1;
	int numServer = socketServer(portPublic+numJoueur,TCP);
	int msgSock = accept(numServer, NULL, NULL);
	write(msgSock, &numJoueur, sizeof(int));

	int signal;
	read(tubeIn, &signal, sizeof(int));
	write(msgSock, &signal, sizeof(int));
	
	shutdown(msgSock, SHUT_RDWR);
	return 0;
}

