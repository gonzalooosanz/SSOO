#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5
#define M 2
#define MBUFFER 10
#define MAXITEMS 70

pthread_t th1[N];
pthread_t th2[M];
pthread_mutex_t mutex;
pthread_cond_t lleno;
pthread_cond_t vacio;

int suma=0;
int Buffer[MBUFFER]; // Buffer de tipo circular 
int nproducciones = 0,nextracciones=0, nElem = 0;	
// Hilo productor
void * fun1(void * i){
	int id_hilo = (int)i;
	int pos = 0, dato;

	while(nproducciones < MAXITEMS){
		dato = id_hilo;
		pthread_mutex_lock(&mutex);

		while(pos == MBUFFER)
			pthread_cond_wait(&lleno, &mutex);

		Buffer[pos] = dato;
		pos = (pos + 1) % MBUFFER;
	   	nproducciones = nproducciones + 1;
		pthread_cond_signal(&vacio);
	    pthread_mutex_unlock(&mutex);
		fprintf(stderr, "Productor %d he añadido el item valor: %d El numero total de producciones es %d \n", id_hilo, dato, nproducciones );
		sleep(random() % 5);	
	}
		
	pthread_exit(0);
}


// Hilo consumidor
void * fun2(){
	//while(1){
	int dato, i ,pos2 = 0;
	while( nextracciones < MAXITEMS - 1){
		pthread_mutex_lock(&mutex); /* acceder al buffer */
		while (nproducciones < nextracciones|| pos2 == MBUFFER || nproducciones == 0) /* si buffer vacio */
			pthread_cond_wait(&vacio, &mutex); /* se bloquea */

		dato = Buffer[pos2];
		suma = suma + dato;
		nextracciones = nextracciones + 1;
		printf("Consumidor %d, valor extraido %d, valor acumulado %d y numero de extracciones %d \n", pos2, dato, suma, nextracciones); /* consume dato */
		pos2 = (pos2 + 1) % MBUFFER;
		pthread_cond_signal(&lleno); /* buffer no lleno */
		pthread_mutex_unlock(&mutex);
		
	}
	
	pthread_exit(0);		
}

int main(){
int i, j;

pthread_mutex_init(&mutex, NULL);
pthread_cond_init(&lleno, NULL);
pthread_cond_init(&vacio, NULL);

// Crear hilos que ejecutan la funcion fun2
for (i = 0; i < N; i++){
	pthread_create(&th1[i], NULL, fun1, (void*) i);
}

for (j = 0; j < M; j++){
	pthread_create(&th2[j], NULL, fun2, NULL);
}

// Esperar terminación hilos
for (i = 0; i < N; i++){
	pthread_join(th1[i], NULL);
}

for (j = 0; j < M; j++){
	pthread_join(th2[j], NULL);
}

printf( " Los valores obtenidos por el hilo 1 son %d \n" , nproducciones);
printf( " Los valores obtenidos por el hilo 2 son %d \n", nextracciones);

pthread_mutex_destroy(&mutex);
pthread_cond_destroy(&lleno);
pthread_cond_destroy(&vacio);

// Mostrar valores
return suma;
}