/* ---------------------------------------------------------------------------
 * Affichage du plateau de jeu, utilise GestionJeu pour déterminer le 
 * caractère à afficher à une position donnée
 * Auteur     : Damien Genthial
12345678901234567890123456789           
            o o =
            o 1 o
            o 2 o
    B B     o 3 o    R R 
    B B     o 4 o    R R 
            o 5 o
= o o o o o o 6 o o o o o o o
o 1 2 3 4 5 6   6 5 4 3 2 1 o
o o o o o o o 6 o o o o o o =
            o 5 o
    V V     o 4 o    J J
    V V     o 3 o    J J 
            o 2 o
            o 1 o
            = o o
 */

#include <stdio.h>
#include "GestionJeu.h"

#define PLATEAU_C
#include "Plateau.h"

static const char CSI[] = {0x1B, '[', 0};

static void clear() { printf("%s2J", CSI); }
static void jumpTo(int row, int column) { printf("%s%d;%dH", CSI, row, column); }
static void up(int nb) { printf("%s%dA", CSI, nb); }
static void down(int nb) { printf("%s%dB", CSI, nb); }
static void right(int nb) { printf("%s%dC", CSI, nb); }
static void left(int nb) { printf("%s%dD", CSI, nb); }
// static void black(void) { printf("%s1;30m", CSI); }
static void red(void) { printf("%s1;31m", CSI); }
static void green(void) { printf("%s1;32m", CSI); }
// static void yellow(void) { printf("%s1;33m", CSI); }
static void blue(void) { printf("%s1;34m", CSI); }
static void magenta(void) { printf("%s1;35m", CSI); }
// static void cyan(void) { printf("%s1;36m", CSI); }
// static void white(void) { printf("%s1;37m", CSI); }
static void normal(void) { printf("%s0m", CSI); }

// Caractère à utiliser pour afficher les chevaux d'une équipe donnée
static char whichCharHorse(Team t, char* nomC, int i, int nbC) {
    char res;
    switch (t) {
        case ROUGE: res = nomC[i]; break;
        case MAGENTA: res = nomC[nbC+i]; break;
        case VERTE: res = nomC[2*nbC+i]; break;
        case BLEUE: res = nomC[3*nbC+1]; break;
    }
    return res;
}

// Type pour toutes les fonctions de sélection de la couleur d'affichage
typedef void (*SelectColor) (void);
// Variable pour la couleur par défaut à un instant donné
SelectColor defaultColor = normal;

// Affichage d'un caractère dans la couleur par défaut ou dans la couleur de l'équipe
// s'il s'agit d'un cheval d'une équipe
static void putColoredChar(char c, int couleur) {
    switch (couleur) {
        case 0: red(); break;
        case 1: magenta(); break;
        case 2: green(); break;
        case 3: blue(); break;
    }
    putchar(c);
    defaultColor();
}

// Les origines permettent de décaler un peu le plateau dans la fenêtre
void affichePlateau(int originRow, int originColumn, int nbJ, int nbC, int* posCh, char* nomChevaux, int* ptrCouleur) {
    int i;
    int pos = 1;
    char tempo;

    clear();
    jumpTo(originRow,originColumn + 17);

    red(); defaultColor = red;
    for (i = 0; i < 7; i++) {
    	tempo=whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur); 
    	putColoredChar(tempo, *ptrCouleur);
    	*ptrCouleur = 0;
    	left(1);down(1); }
    	
    up(1); right(1);
    for (i = 0; i < 6; i++) {
    	putchar(' ');
    	tempo=whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur); 
    	putColoredChar(tempo, *ptrCouleur);
    	*ptrCouleur = 0; }
    	
    down(1); left(1);
    tempo=whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur); 
    putColoredChar(tempo, *ptrCouleur);
    *ptrCouleur = 1;
    left(1); down(1);

    magenta(); defaultColor = magenta;
    for (i = 0; i < 6; i++) { 
    	tempo=whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur); 
    	putColoredChar(tempo, *ptrCouleur);
    	*ptrCouleur = 1;
    	left(3); }
    for (i = 0; i < 7; i++) { 
    	tempo=whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur); 
    	putColoredChar(tempo, *ptrCouleur);
    	*ptrCouleur = 1;
    	left(1); down(1); }
    up(1); left(2);
    tempo=whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur); 
    putColoredChar(tempo, *ptrCouleur);
    *ptrCouleur = 2;
    left(3);

    green(); defaultColor = green;
    for (i = 0; i < 7; i++) { 
    	tempo=whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur); 
    	putColoredChar(tempo, *ptrCouleur);
    	*ptrCouleur = 2;
    	left(1); up(1); }
    down(1); left(2);
    for (i = 0; i < 6; i++) { 
    	tempo=whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur); 
    	putColoredChar(tempo, *ptrCouleur);
    	*ptrCouleur = 2;
    	left(3); }
    up(1); right(2);
    tempo=whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur); 
    	putColoredChar(tempo, *ptrCouleur);
    *ptrCouleur = 3;
    left(1); up(1);

    blue(); defaultColor = blue;
    for (i = 0; i < 7; i++) { 
    	tempo=whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur); 
    	putColoredChar(tempo, *ptrCouleur);
    	*ptrCouleur = 3;
    	putchar(' '); }
    left(2); up(1);
    for (i = 0; i < 5; i++) { 
    	tempo=whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur); 
    	putColoredChar(tempo, *ptrCouleur);
    	*ptrCouleur = 3;
    	left(1); up(1); }
    tempo=whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur); 
    	putColoredChar(tempo, *ptrCouleur);
    *ptrCouleur = 3;
    putchar(' ');
    tempo=whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur); 
    	putColoredChar(tempo, *ptrCouleur);
    *ptrCouleur = 0;

    // Escaliers
    left(1); down(1);
    red(); pos = 61;
    for (i = 0; i < 6; i++) { putchar(whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur)); left(1); down(1); }
    
    right(12);
    magenta(); pos = 71;
    for (i = 0; i < 6; i++) { putchar(whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur)); left(3); }
    
    down(6);
    green(); pos = 81;
    for (i = 0; i < 6; i++) { putchar(whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur)); left(1); up(1); }
    
    left(12);
    blue(); pos = 91;
    for (i = 0; i < 6; i++) { putchar(whichChar(pos++, nbC, nbJ, posCh, nomChevaux, ptrCouleur)); putchar(' '); }

    // Écuries
    jumpTo(originRow + 3, originColumn + 21);
    red();
    for (i = 0; i < nbHorsesHome(ROUGE, nbC, posCh); i++) {
        if (i == 2) { down(1); left(4); }
        putchar(whichCharHorse(ROUGE, nomChevaux, i, nbC)); putchar(' ');
    }
    jumpTo(originRow + 10, originColumn + 21);
    magenta();
    for (i = 0; i < nbHorsesHome(MAGENTA, nbC, posCh); i++) {
        if (i == 2) { down(1); left(4); }
        putchar(whichCharHorse(MAGENTA, nomChevaux, i, nbC)); putchar(' ');
    }
    jumpTo(originRow +10, originColumn + 6);
    green();
    for (i = 0; i < nbHorsesHome(VERTE, nbC, posCh); i++) {
        if (i == 2) { down(1); left(4); }
        putchar(whichCharHorse(VERTE, nomChevaux, i, nbC)); putchar(' ');
    }
    jumpTo(originRow + 3, originColumn + 6);
    blue();
    for (i = 0; i < nbHorsesHome(BLEUE, nbC, posCh); i++) {
        if (i == 2) { down(1); left(4); }
        putchar(whichCharHorse(BLEUE, nomChevaux, i, nbC)); putchar(' ');
    }

    normal(); jumpTo(20, 1); putchar('\n');
}

