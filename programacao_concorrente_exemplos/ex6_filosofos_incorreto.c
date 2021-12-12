#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <semaphore.h>

#define N 5  /* qtdade de filosofos */
#define TAKEN 1
#define NOT_TAKEN 0 


sem_t mutex;                //exclusao mutua para regioes cri­ticas

//array para controlar o estado dos garfos
int   forks[N];        

//uma thread para cada filósofo
pthread_t tid[N];

/* i: numero do filosofo, vai de 0 a N-1 */


void take_fork(int f, int g);
void put_fork(int f, int g);

void think(int i);

void eat(int i);


void philosopher(int i) {

	while(1) {  /* repete eternamente */

 		think(i);  /* o filosofo esta pensando */
		
 		take_fork(i, i); /* pega o garfo esquerdo*/
		
		eat(i);  /* come espaguete */

 		put_fork(i, i); /* coloca o garfo de volta na mesa */
 		put_fork(i, (i+1)%N); /* coloca o garfo de volta na mesa */
		pthread_yield();
	}
}

void think(int i) {
	/*Filosofo esta pensando...*/
	//sleep(1);
	return;
}

void take_fork(int f, int g) {

	while(1){
		while(forks[g] != NOT_TAKEN);/*Espera o garfo da esquerda ficar disponivel*/	
		forks[g] = TAKEN;
		printf("Filosofo#%d pegou o  garfo %d; forks[%d]=%d\n",f, g, g, forks[g]);
		if(forks[(g+1)%N] == TAKEN){
			forks[g] = NOT_TAKEN;
			printf("\tFilosofo#%d liberou o  garfo %d; forks[%d]=%d\n",f, g, g, forks[g]);
			printf("\t\tFILOSOFO%d ESTA COM FOME!\n", f );
			continue;
		}
		else{
			forks[(g+1)%N] = TAKEN;
			printf("Filosofo#%d pegou o  garfo %d; forks[%d]=%d\n",f, (g+1)%N, g, forks[g]);
			break;
		}
	}
		
	


}

void eat(int i) {
	/*Filosofo esta comendo...*/
	//sleep(1);
	printf("\t\tFILOSOFO%d COMEU!\n", i );
	return;
}

void put_fork(int f, int g) {
	forks[g] = NOT_TAKEN;
	printf("\tFilosofo#%d liberou o  garfo %d; forks[%d]=%d\n",f, g, g, forks[g]);
}




int main(int argc, char *argv[])
{

	int i;
	int p[N] ;

     	for(i= 0; i < N ;i++ ){
		forks[i] = NOT_TAKEN;
	}


     	for(i= 0; i < N ;i++ ){
		p[i] = i;
	}
	

	sem_init(&mutex, 0, 1);

	for(i=0;i<N;i++)
		pthread_create( &tid[i], NULL,(void *) philosopher, (int*) (size_t)p[i]);

	for(i=0;i<N;i++)
		pthread_join( tid[i], NULL);

	sem_destroy(&mutex);

	exit(0);
}

