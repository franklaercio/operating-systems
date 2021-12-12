#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <semaphore.h>

#define N 5									 /* qtdade de filosofos */
#define LEFT (i + N - 1) % N /* calculo do vizinho a esquerda de i */
#define RIGHT (i + 1) % N		 /* calculo do vizinho a direita de i */
#define THINKING 0					 /* filosofo pensando */
#define HUNGRY 1						 /* filosofo tentando pegar garfos */
#define EATING 2						 /* filosofo comendo */
#define TRUE 1

sem_t s[N];	 //um semaforo por filosofo
sem_t mutex; //exclusao mutua para regioes cri­ticas

//array para controlar o estado dos filosofos
int state[N];

//uma thread para cada filósofo
pthread_t tid[N];

/* i: numero do filosofo, vai de 0 a N-1 */

void take_forks(int i);
void put_forks(int i);

void test(int i);
void think(int i);

void eat(int i);

void philosopher(int i)
{

	while (1)
	{ /* repete eternamente */

		think(i); /* o filosofo esta pensando */

		take_forks(i); /* pega dois garfos ou bloqueia */

		eat(i); /* come espaguete */

		put_forks(i); /* coloca os dois garfos de volta na mesa */
	}
}

void think(int i)
{
	/*Filosofo esta pensando...*/
	//sleep(1);
	printf("\tFilosofo#%d esta PENSANDO\n", i);
	return;
}

void take_forks(int i)
{

	sem_wait(&mutex);	 //down(&mutex); /* entra na regiao critica */
	state[i] = HUNGRY; /* registra que o filosofo i esta com fome */
	printf("Filosofo#%d esta com FOME\n", i);
	test(i); /* tenta pegar 2 garfos */

	sem_post(&mutex); //up(&mutex);  /* sai da regiao cri­tica */
	sem_wait(&s[i]);	//down(&s[i]); /* bloqueia se os garfos nao foram pegos */
}

void eat(int i)
{
	/*Filosofo esta comendo...*/
	//sleep(1);
	printf("\tFilosofo#%d esta COMENDO\n", i);

	return;
}

void put_forks(int i)
{
	sem_wait(&mutex);		 //down(&mutex); /* entra na regiao critica */
	state[i] = THINKING; /* o filosofo acabou de comer */
	printf("Filosofo#%d agora vai PENSAR\n", i);
	printf("Filosofo#%d verificando se Filosofo#%d pode comer\n", i, LEFT);
	test(LEFT); /* verifica se o vizinho da esquerda pode comer agora */
	printf("Filosofo#%d verificando se Filosofo#%d pode comer\n", i, RIGHT);
	test(RIGHT);			/* verifica se o vizinho da direita pode comer agora */
	sem_post(&mutex); //up(&mutex);  /* sai da regiao cri­tica */
}

void test(int i)
{ //testa se os filosofos vizinhos podem comer
	if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
	{
		state[i] = EATING;
		printf("Filosofo#%d esta COMENDO (%d e %d nao podem comer)\n", i, LEFT, RIGHT);
		sem_post(&s[i]); //up(&s[i]);
	}
}

int main(int argc, char *argv[])
{

	int i;
	int p[N];

	//inicialização dos semáforos...
	for (i = 0; i < N; i++)
	{
		sem_init(&s[i], 0, 1);
		p[i] = i;
	}

	sem_init(&mutex, 0, 1);

	for (i = 0; i < N; i++)
		pthread_create(&tid[i], NULL, (void *)philosopher, (int *)(size_t)p[i]);

	for (i = 0; i < N; i++)
		pthread_join(tid[i], NULL);

	exit(0);
}
