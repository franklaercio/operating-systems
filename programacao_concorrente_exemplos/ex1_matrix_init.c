#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#define NT 10
#define SIZE 10

pthread_mutex_t mutex;
int counter=1;
int matriz[NT][SIZE];

void imprime_matriz(){
	int i, j;
	for(i=0;i<NT;i++){
		for(j=0;j<SIZE;j++){
			printf("%d\t", matriz[i][j]);
		}
		printf("\n");
	}
}

void* inicializa_matriz(void *p){
	int i, j, n;
	n = (int)(size_t)p;
	pthread_mutex_lock(&mutex);	
	for(j=0;j<NT;j++)
		matriz[n][j] = rand()%100;

	pthread_mutex_unlock(&mutex);	
	pthread_exit(NULL);
}

int main(int argc, char **argv) {
  int i;
  pthread_t tid[SIZE];

  pthread_mutex_init(&mutex, NULL);

  srand(time(NULL));


  for(i=0;i<NT;i++)
  	pthread_create(&tid[i], NULL, inicializa_matriz, (void *)(size_t) i);
  for(i=0;i<NT;i++)
  	pthread_join(tid[i], NULL);

  imprime_matriz();

  pthread_mutex_destroy(&mutex);	

  return 0;

}
