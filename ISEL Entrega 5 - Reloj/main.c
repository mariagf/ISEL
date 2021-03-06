#include <stdio.h>
#include <time.h>
#include "fsm.h"

int segundos;
int minutos;
int horas;

enum reloj {
  INICIAR,
  PINTAR,
  FINALIZAR,
};

int sensor;
int flagLlegada;
int arrayFecha[8];

static void simbolos(int var){
switch (var) {
	case 0:
		printf(" __ __ \n");
		printf("|__ __|\n");
	break;

	case 1:
		printf("       \n");
		printf(" __ __ \n");
	break;

	case 2:
		printf(" __    \n");
		printf("|  |__|\n");
	break;

	case 3:
		printf("       \n");
		printf("|__|__|\n");
	break;

	case 4:
		printf("    __ \n");
		printf(" __|__ \n");
	break;

	case 5:
		printf("    __ \n");
		printf("|__|  |\n");
	break;
	case 6:
		printf(" __ __ \n");
		printf("|__|  |\n");
	break;
	case 7:
		printf("       \n");
		printf(" __ __|\n");
	break;
	case 8:
		printf(" __ __ \n");
		printf("|__|__|\n");

	break;	
	case 9:
		printf("    __ \n");
		printf("|__|__|\n");
	break;
	default:
	break;
	}
}

void actualizarFecha(){

	time_t fechaOrigen = time(NULL); //epoch 00:00 del 1 de enero de 1970 cuenta los segundos desde entonces.

	struct tm fecha = *localtime(&fechaOrigen); // Seprara minutos horas etc..
	
	segundos = fecha.tm_sec;
	minutos = fecha.tm_min;
	horas = fecha.tm_hour;
	
	arrayFecha[0] = horas/10;
	arrayFecha[1] = horas%10;
	arrayFecha[3] = minutos/10;
	arrayFecha[4] = minutos%10;
	arrayFecha[6] = segundos/10;
	arrayFecha[7] = segundos%10;
	
}

static int sensorSalida ()
{
	if (sensor == 1){
		return 1;
	} else{
		return 0;
	}
}
static void espera ()
{
	struct timespec tiempoEspera = {0, 500000000};
	nanosleep(&tiempoEspera, NULL);

}

static void pinta ()
{
	int i;
	actualizarFecha();
	for (i=0; i<8; i++){
		espera();
		if (i==2 || i==5){
			printf("  · ·  \n");
		}
		else{
			simbolos(arrayFecha[i]);
		}
	}

	flagLlegada = 1;

}

static int sensorLlegada ()
{
	if (flagLlegada == 1){
		flagLlegada = 0;
		return 1;
	} else{
		return 0;
	}
	
}

static void fin ()
{
	printf("\n");
}

static fsm_trans_t reloj[] = {
  { INICIAR, sensorSalida, PINTAR,     pinta    },
  { PINTAR,  sensorLlegada, INICIAR,    fin   },
  {-1, NULL, -1, NULL },
};

int main (){
	fsm_t* reloj_fsm = fsm_new(reloj);

	while(scanf("%d", &sensor)==1){
		fsm_fire(reloj_fsm);	
	}
return 0;
}
