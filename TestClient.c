#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#define MAX 256

#include "Plateau.h"
#include "GestionJeu.h"
#include "Reseau.h"

int main(int nbArgs, char* arg[])
{
	int numPortEnv = 1234;
	int numJoueurEnv = 1;
	
	int numServer = socketServer(7777,TCP);
	int numServer1234 = socketServer(1234,TCP);
	
	int msgSock = accept(numServer, NULL, NULL);
	printf("Qq se connecte sur le public\n");
	
	write(msgSock, &numPortEnv, sizeof(int));
	printf("J'ai ecrit %d \n" , numPortEnv);
	
	int msgSock1234 = accept(numServer1234, NULL, NULL);
	printf("Qq se connecte sur le privé 1234\n");
	
	write(msgSock1234, &numJoueurEnv, sizeof(int));
	printf("J'ai ecrit %d \n" , numJoueurEnv);
	
	shutdown(msgSock, SHUT_RDWR);
	shutdown(msgSock1234, SHUT_RDWR);
	return EXIT_SUCCESS;
}



