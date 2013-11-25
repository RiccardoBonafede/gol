#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "main.h"

#define ON 'x'
#define OFF ' '


struct node **table;	//table[y][x]

	
void init(void);
void getLife(void);
void printTable(void);
short int countNeightboor(short x, short y);
void next(void);
int main(int argc, char *argv[]){
	
	
	initscr();      /* inizializza la libreria curses */ 
  	keypad(stdscr, TRUE);  /* abilita la mappatura della tastiera */ 
    	nonl();         /* non convertire NL->CR/NL in output */ 
   	//cbreak();       /* prende i caratteri in input uno alla volta, senza attendere il \n */ 
    	noecho(); 
	curs_set(0);
	mousemask(ALL_MOUSE_EVENTS, NULL);
	init();
	getLife();
	
}

void init(void){
	int i=0, a=0;
	if((table= malloc(LINES*sizeof( struct node*)))==NULL){
		fprintf(stderr, "ERROR: Out if memory\n");	
	
	};
	
	while(i<=LINES){
		a=0;
		if((table[i]=(struct node*)malloc(COLS*sizeof(struct node)))==NULL){
			fprintf(stderr, "ERROR: Out if memory\n");
		}	
		while(a<=COLS){
			table[i][a].y=i;
			table[i][a].x=a;
			table[i][a].on=OFF;
			a++;
		}
		i++;
	}
	
	refresh();
	
}
void getLife(void){
	int key;
	MEVENT event;


	while(1){
		key=getch();
		switch(key){
			case KEY_MOUSE:
				if(getmouse(&event) == OK){
					if(event.bstate & BUTTON1_CLICKED){
						if(table[event.y][event.x].on==ON){
							table[event.y][event.x].on=OFF;
						}
						else if(table[event.y][event.x].on==OFF){
							table[event.y][event.x].on=ON;
						};
					
						
					}
				}
				break;
			case 'n':
				
				next();
				break;
		}
		printTable();
		key=0;
		usleep(100);
	};
	
	
}
void printTable(void){
	int i=0, a=0;
	
	while(i<LINES){
		a=0;
		while(a<COLS){
			mvaddch(i,a,table[i][a].on);
			table[i][a].last=table[i][a].on;
			a++;
			
		}
		i++;
	}

	refresh();
}
short int countNeightboor(short x, short y){
	short int c=0,i=0;
	struct node offset[8]={
		{-1,-1, ' '},
		{-1, 0, ' '},
		{-1, 1, ' '},
		{0, -1, ' '},
		{0, 1, ' '},
		{1, -1, ' '},
		{1, 0, ' '},
		{1, 1, ' '}
	};
	/*
	###	uso le matrici soprastanti per determinare il numero dei figli...
	#X#	Cosi' mi risparmio 8 if di cacca pieni di merda.
	###	N.B. : risolto problema di x,y == 0
		dio can del can, fuori di uno
	*/
	
	while(i<8){
		
		if((y+offset[i].y)<0 || (y+offset[i].y)>LINES){
				i++;
				continue;
		}
		if((x+offset[i].x)<0 || (x+offset[i].x)>COLS){
				i++;
				continue;	
		}
		if(table[y+offset[i].y][x+offset[i].x].last==ON){
			c++;
		}
		i++;
	}
	if(c>8)
		fprintf(stderr, "Somethings gone wrong in CountNeightboor");
	return c;
}
void next(void){
	short i=0, a=0,c=0;
	
	
	while(i<LINES){
		a=0;	
		while(a<COLS){
			
			c=countNeightboor(a, i);
			
			if(c==3 && table[i][a].on==OFF){	//NASCE
				table[i][a].on=ON;
			}
			else if(!(c==2 || c==3) && table[i][a].on==ON){//CREPA
				table[i][a].on=OFF;
				
			}
			a++;
		}
		i++;
	}	
}
