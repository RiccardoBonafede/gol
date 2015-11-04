
//Librerie necessarie

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

/*Strutture necessarie Al programma*/
struct node{
	short int x;
	short int y;	/*per applicazioni future*/
	char on;
	char last;
	unsigned long int t; /*come sopra,contatore della generazione corrente*/
	};

struct table_size
{
	unsigned short int l;/* numero di celle (equivalente a LINE a inizio esecuzione*/
	unsigned short int c; /* Dimensione celle. Equivalente al numero di colonne a inizio esecuzione*/
};

//Direttive al preprocessore

#define ON 'x'
#define OFF ' '