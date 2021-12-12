#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>

#define MAX 100000000
pthread_mutex_t mutex;
pthread_cond_t condc, condp;
int buffer = 0;

void* produtor(void *ptr) {
  int i;

  for (i = 1; i <= MAX; i++) {
    pthread_mutex_lock(&mutex);
    while (buffer != 0)	{	      
      printf("PRODUTOR: Buffer CHEIO. Indo dormir\n");
      pthread_cond_wait(&condp, &mutex);
    }
    printf("PRODUTOR: Acordado! Produzindo...\n");
    buffer = i;
    pthread_cond_signal(&condc);	
    pthread_mutex_unlock(&mutex);	
  }
  pthread_exit(0);
}

void* consumidor(void *ptr) {
  int i;

  for (i = 1; i <= MAX; i++) {
    pthread_mutex_lock(&mutex);
    while (buffer == 0){			
      printf("CONSUMIDOR: Buffer VAZIO. Indo dormir\n");
      pthread_cond_wait(&condc, &mutex);
    }
    printf("CONSUMIDOR: Acordado! Consumindo...\n");
    buffer = 0;
    pthread_cond_signal(&condp);	
    pthread_mutex_unlock(&mutex);	
  }
  pthread_exit(0);
}

int main(int argc, char **argv) {
  pthread_t pro, con;

  pthread_mutex_init(&mutex, NULL);	
  pthread_cond_init(&condc, NULL);		
  pthread_cond_init(&condp, NULL);	

  pthread_create(&con, NULL, consumidor, NULL);
  pthread_create(&pro, NULL, produtor, NULL);

  pthread_join(con, NULL);
  pthread_join(pro, NULL);

  pthread_mutex_destroy(&mutex);	
  pthread_cond_destroy(&condc);		
  pthread_cond_destroy(&condp);	

}
