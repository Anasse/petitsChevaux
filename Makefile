# Makefile pour le projet de jeu des petits chevaux en ligne
# TODO : à compléter

CC = gcc --std=c99 -Wall -g -c
LD = gcc

PROG=testPlateau coteClient coteServ
all: $(PROG)

clean:
	rm -f *~ *.o $(PROG)

coteClient : coteClient.o Reseau.o
	$(LD) -o coteClient coteClient.o Reseau.o

coteServ : coteServ.o Reseau.o
	$(LD) -o coteServ coteServ.o Reseau.o

testPlateau: testPlateau.o Plateau.o GestionJeu.o
	$(LD) -o testPlateau testPlateau.o Plateau.o GestionJeu.o



GestionJeu.o: GestionJeu.c GestionJeu.h
	$(CC) GestionJeu.c

Plateau.o: Plateau.c Plateau.h GestionJeu.h
	$(CC) Plateau.c


Reseau.o: Reseau.c Reseau.h
	$(CC) Reseau.c 

coteClient.o : coteClient.c Reseau.h
	$(CC) coteClient.c 

coteServ.o : coteServ.c Reseau.h
	$(CC) coteServ.c
