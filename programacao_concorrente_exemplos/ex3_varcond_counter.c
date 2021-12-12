#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#define SIZE 100

pthread_mutex_t mutex;
pthread_cond_t limite, verifica;
int counter=0;
int vetor[SIZE];


void* imprime(void* p){
	int i;
	while(1){
		pthread_mutex_lock(&mutex);
		if(!counter ||  counter%10!=0){
			
			printf("-------- AGUARDANDO ADICIONAR\n");
			pthread_cond_wait(&verifica, &mutex);
		}
		printf("=====================================\n");
		for(i=counter-10;i<counter;i++)
			printf("vetor[%d]=%d\n", i, vetor[i]);
		printf("=====================================\n");
		counter -= 10;
		pthread_cond_signal(&limite);
	
		pthread_mutex_unlock(&mutex);
		if(counter>=100)
      			pthread_exit(NULL);
	}
}

void* adiciona(void *p){

      while(1){
	pthread_mutex_lock(&mutex);
	
	vetor[counter] = rand()%100;
	counter++;

	if(counter && counter%10==0){
		printf("+++++++ AGUARDANDO IMPRESSAO...\n");
		pthread_cond_signal(&verifica);
		pthread_cond_wait(&limite, &mutex);
	}
	
	pthread_mutex_unlock(&mutex);
	if(counter>=100)
      		pthread_exit(NULL);
      }


}

int main(int argc, char **argv) {
  int i;
  pthread_t tid[SIZE];

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&limite, NULL);
  pthread_cond_init(&verifica, NULL);

  srand(time(NULL));


  	pthread_create(&tid[0], NULL, adiciona, (void *)(size_t) 0);
  	pthread_create(&tid[1], NULL, imprime, (void *)(size_t) 1);
  	
	pthread_join(tid[0], NULL);
  	pthread_join(tid[1], NULL);


  pthread_mutex_destroy(&mutex);	
  pthread_cond_destroy(&limite);	
  pthread_cond_destroy(&verifica);	

  return 0;

}
