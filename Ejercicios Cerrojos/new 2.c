
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5
#define M 2
#define MBUFFER 10
#define MAXITEMS 20

volatile int suma = 0;
volatile int Buffer[MBUFFER];
volatile int rp = 0;
volatile int wp = 0;
volatile int nproducciones = 0;
volatile int nextracciones = 0;

pthread_mutex_t mtx;
pthread_cond_t notempty;
pthread_cond_t notfull;

int buffer_full(void)
{
	return wp == ((rp + 1) % MBUFFER);
}

int buffer_empty(void)
{
	return wp == rp;
}

void buffer_insert(int elem)
{
	Buffer[wp] = elem;
	wp = (wp + 1) % MBUFFER;
}

int buffer_extract(void)
{
	int elem;
	elem = Buffer[rp];
	rp = (rp + 1) % MBUFFER;
	return elem;
}

void *fun1(void *arg)
{
	int arg1 = (int) arg;
	int done = 0;

	printf("Hilo productor con id %lu y argumento %d\n", pthread_self(), arg1);

	while (!done) {
		pthread_mutex_lock(&mtx);
		while ((nproducciones < MAXITEMS) && buffer_full()) {
			pthread_cond_wait(&notfull, &mtx);
		}
		if (nproducciones < MAXITEMS) {
			buffer_insert(arg1);
			nproducciones++;
			fprintf(stderr,"Productor %lu, he añadido un item %d. "
					"El número total de producciones es %d\n", pthread_self(),
					arg1, nproducciones);
			pthread_cond_signal(&notempty);
		}
		if (nproducciones == MAXITEMS) {
			pthread_cond_broadcast(&notfull);
			pthread_cond_broadcast(&notempty);
			done = 1;
		}
		pthread_mutex_unlock(&mtx);
		if (!done)
			sleep(random() % 5);
	}

	pthread_exit(NULL);
}

void *fun2(void *arg)
{
	int done = 0;
	int elem;

	while (!done) {
		pthread_mutex_lock(&mtx);
		while ((nextracciones < MAXITEMS) && buffer_empty()) {
			pthread_cond_wait(&notempty, &mtx);
		}
		if (nextracciones < MAXITEMS) {
			elem = buffer_extract();
			nextracciones++;
			suma += elem;
			fprintf(stderr, "Consumidor %lu, valor extraido %d, acumulado %d, "
					"extracciones %d\n",
					pthread_self(), elem, suma, nextracciones);
			pthread_cond_signal(&notfull);
		}
		if (nextracciones == MAXITEMS)
			done = 1;
		pthread_mutex_unlock(&mtx);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int i;
	pthread_t tids[N + M];

	// inicialización de mutex y variables de condición
	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&notfull, NULL);
	pthread_cond_init(&notempty, NULL);

	// Creación de hilos que ejecutan fun1
	for (i = 0; i < N; i++)
		pthread_create(&tids[i], NULL, fun1, (void *)(i + 1));

	// Creación de hilos que ejecutan fun2
	for (i = N; i < N + M; i++)
		pthread_create(&tids[i], NULL, fun2, NULL);

	// Espera a la finalización de los hilos
	for (i = 0; i < N + M; i++)
		pthread_join(tids[i], NULL);

	return suma;
}
ejercicioA.c
Mostrando Parte2.txt.