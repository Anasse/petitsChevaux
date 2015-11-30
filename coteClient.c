#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "Plateau.h"
#include "GestionJeu.h"
#include "Reseau.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#define MAX 256

int main(){
	
	char tampon[MAX];
	int numClient = socketClient("localhost", 7777, TCP);
	int nb = read(numClient, tampon, MAX);
	write(1, tampon, nb);
	printf("%s\n" , tampon);

	 close(numClient);
	return EXIT_SUCCESS;
}
