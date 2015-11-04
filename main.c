/*
	@author: Riccardo Bonafede
	https://keybase.io/bonaff

*/


#include "main.h"


struct node **table;	//table[y][x]
struct table_size memory_usage;


	
void init(void);
void getLife(void);
void printTable(void);
short int countNeightboor(short x, short y);
void next(void);
void quit(void);

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


	memory_usage.l=LINES + 1;
	memory_usage.c=COLS + 1;

	table= (struct node**) malloc((memory_usage.l) * sizeof( struct node*));
	

	if(table==NULL){
		endwin();
		fprintf(stderr, "Fatal: Out if memory\n");
		exit(EXIT_FAILURE);
	
	};
	
	while(i<=memory_usage.l){
		a=0;

		table[i]= (struct node*) malloc((memory_usage.c) * sizeof(struct node));

		if(table[i]==NULL){
			endwin();
			fprintf(stderr, "Fatal: Out if memory\n");
			exit(EXIT_FAILURE);
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

		switch(key){ //andrebbe aggiunto il supporto a più funzioni(vedi auto play, pausa, salva, ecc)

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

			case 'q':

				quit();
				exit(EXIT_SUCCESS);
				break;

			default:
				//donothing
				break;
		}

		printTable();
		key=0;
		usleep(100);
	};
}

void printTable(void){
	int i=0, a=0;
	
	while(i<memory_usage.l){
		a=0;

		while(a<memory_usage.c){
										//salva tutti i caratteri nel buffer, e fa un refresh alla schermata 
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
		//tolte un po' di parolacce
	*/
	
	while(i<8){
		if((y+offset[i].y)<0 || (y+offset[i].y)>memory_usage.l){ 
				i++;
				continue;
		}
		if((x+offset[i].x)<0 || (x+offset[i].x)>memory_usage.c){
				i++;
				continue;	
		}
		if(table[y+offset[i].y][x+offset[i].x].last==ON){
			c++;
		}
		i++;
	}
	if(c>8){
		fprintf(stderr, "Fatal: Somethings gone wrong in CountNeightboor"); //Questo non dovrebbe accadere
		endwin();
		exit(EXIT_FAILURE);
	}
	return c;
}

void next(void){
	short i=0, a=0,c=0;
	
	while(i<memory_usage.l){
		a=0;

		while(a<memory_usage.c){
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

void quit(){
	int a=0;

 	endwin();

	while(a<memory_usage.l){
		free(table[a]);
		a++;
	}

	free((void **)table);
	return;
}
