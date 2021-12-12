#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#define NT 10
#define SIZE 100

pthread_mutex_t mutex;
int matriz[SIZE][NT];

int inicializa_matriz(){
	int i, j;
	for(i=0;i<SIZE;i++)
		for(j=0;j<NT;j++)
			matriz[i][j] = rand()%100;
	return 0;
}

void* soma_coluna(void *p) {
  int i, soma=0;
  pthread_mutex_lock(&mutex);

  for(i=0;i<SIZE;i++){
		
	  soma += matriz[i][(int)(size_t)p];
  }

  pthread_mutex_unlock(&mutex);

  printf("THREAD#%d: Finalizou. SOMA=%d\n", (int)(size_t)p, soma);	
  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  int i;
  pthread_t tid[SIZE];

  srand(time(NULL));
  inicializa_matriz();

  pthread_mutex_init(&mutex, NULL);	

  for(i=0;i<NT;i++)
  	pthread_create(&tid[i], NULL, soma_coluna, (void *)(size_t) i);
  for(i=0;i<NT;i++)
  	pthread_join(tid[i], NULL);

  pthread_mutex_destroy(&mutex);	

  return 0;

}
