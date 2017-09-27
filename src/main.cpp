#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "kbhit.cpp"

#define LARGO 99
#define VELMAX 100
#define VELMIN 200
#define M_ANCHO 78
#define M_ALTO 23
#define MAPA1 "data/mapa1.map"

int control(void);

int main (void){
	FILE *archivo;
	
	int V[2]={1,0}, aux, f=4, cont, cont2, vel=VELMIN, n=2,
	xg[100]={37},
	yg[100]={18},
	xc, yc,
	mapa_bits_x[M_ALTO][M_ANCHO],
	mapa_bits_y[M_ALTO][M_ANCHO];

	srandom(11);
	
	xc=random()%77; yc=random()%21+1;

	archivo=fopen(MAPA1, "r");
	
	for(cont2=0;cont2<M_ALTO;cont2++){
		for(cont=0;cont<M_ANCHO;cont++)
			fscanf(archivo, "%i,",&mapa_bits_x[cont2][cont]);
		fscanf(archivo, "\n");
	}
	
	rewind(archivo);
	for(cont2=0;cont2<M_ALTO;cont2++){
		for(cont=0;cont<M_ANCHO;cont++)
			fscanf(archivo, "%i,",&mapa_bits_y[cont2][cont]);
		fscanf(archivo, "\n");
	}
	
	for(cont2=0;cont2<M_ALTO;cont2++)
		for(cont=0;cont<M_ANCHO;cont++)
			if(mapa_bits_x[cont2][cont]==1&&mapa_bits_y[cont2][cont]==1){
				mapa_bits_x[cont2][cont]=cont+1;
				mapa_bits_y[cont2][cont]=cont2+1;
			}

	
	initscr();
	curs_set(0);
	noecho();
	
	for(cont2=0;cont2<M_ALTO;cont2++)
		for(cont=0;cont<M_ANCHO;cont++)
			if(mapa_bits_x!=0&&mapa_bits_y!=0)
				mvprintw(mapa_bits_y[cont2][cont],mapa_bits_x[cont2][cont],"#");
	refresh();
	
	while(V[0]){
		V[1]=0;
		mvprintw(yc, xc,"*");
	
		if(kbhit()){
			f=control();
		//validacion de datos: no se validara un cursor inverso
		//al cursor pulsado anteriormente 
			if((f==4&&aux==3)||(f==3&&aux==4)) f=aux;
			if((f==1&&aux==2)||(f==2&&aux==1)) f=aux;
		}

		switch(f){
			case 0: V[0]=0;
			break;
			case 1: aux=f; yg[0]--;
			break;
			case 2: aux=f; yg[0]++;
			break;
			case 3: aux=f; xg[0]++;
			break;
			case 4: aux=f; xg[0]--;
			break;
			default: if(aux==1) yg[0]--;
								if(aux==2) yg[0]++;
								if(aux==3) xg[0]++;
								if(aux==4) xg[0]--;
		}
		//limites para que el gusano
		//traspase la pantalla
		if(xg[0]==0) xg[0]=78;
		if(xg[0]==79) xg[0]=1;
		if(yg[0]==0) yg[0]=23;
		if(yg[0]==24) yg[0]=1;

		//traspazo de datos en cola
		//para tener la trayectoria del gusano
		mvprintw(yg[n],xg[n]," ");
		for(cont=LARGO;cont>0;cont--){
			xg[cont]=xg[cont-1]; yg[cont]=yg[cont-1];
		}
		//mostrar gusano
		mvprintw(yg[0],xg[0],"*");
		refresh();
		
		//velocidad de desplazamiento del gusano
		usleep(vel * 1000);
		
		//verificar si el gusano esta en la misma posicion x y del alimento
		if ((xg[0]==xc)&&(yg[0]==yc)){
			n++;
			if(vel>VELMAX)
				vel-=20;
			do{
				xc=random()%77;
				yc=random()%21+1;

				//asegurarse que la comida no este dentro de los muros del mapa
				for(cont2=0;cont2<M_ALTO;cont2++)
					for(cont=0;cont<M_ANCHO;cont++)
						if(xc==mapa_bits_x[cont2][cont]&&yc==mapa_bits_y[cont2][cont]){
							V[1]=1; 
							cont2=M_ALTO; 
							cont=M_ANCHO;
						}
						
			}while(V[1]);
		}

		for(cont=0;cont<n;cont++)
			if(n==n+1)
				if((xg[0]==xg[cont+1]&&yg[0]==yg[cont+1])&&cont>0){
					f=0;
					break;
				}
		
		for(cont2=0;cont2<M_ALTO;cont2++)
			for(cont=0;cont<M_ANCHO;cont++)
				if(xg[0]==mapa_bits_x[cont2][cont]&&yg[0]==mapa_bits_y[cont2][cont]){
					f=0;
					break;
				}
	}
	fclose(archivo);
	erase();
	mvprintw(12, 39, "JUEGO TERMINADO");
	getch();
	endwin();
	return 0;
}
		
int control(void){
	char ctr;
	do{
		move(23,79);
		ctr = getch();
		if(ctr=='A') return 1;
		if(ctr=='B') return 2;
		if(ctr=='C') return 3;
		if(ctr=='D') return 4;
		if((ctr=='W')||(ctr=='w')) return 10;
		if((ctr=='S')||(ctr=='s')) return 11;
		if((ctr=='D')||(ctr=='d')) return 12;
		if((ctr=='A')||(ctr=='a')) return 13;
		if(ctr=='p') return 0;
	}while(1);
}
