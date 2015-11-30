#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#define MAX 10

#include "Plateau.h"
#include "GestionJeu.h"
#include "Reseau.h"
int main(int nbArgs, char* arg[])
{
	char tampon[MAX] = "ab";
	int numServer = socketServer(7777,TCP);
	int msgSock = accept(numServer, NULL, NULL);
	write(msgSock, tampon, strlen(tampon)+1);
	printf("J'ai ecrit %s \n" , tampon);
	
	shutdown(msgSock, SHUT_RDWR);
	return EXIT_SUCCESS;
}



