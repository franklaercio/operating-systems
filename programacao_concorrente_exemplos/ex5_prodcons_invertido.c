#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#define SHARED 0


sem_t empty, full;    
int *data;             
int numIters, BUFFER_SIZE;
int count_c, count_p;
pthread_mutex_t mutex;

int imprime_buffer(){
	int i;
	printf("POSICAO\t|\tCONTEUDO\n");
	for(i=0;i<BUFFER_SIZE;i++){
		printf("[%d]\t|\t%d\n", i, data[i]);
	}
	return 0;
}

void *produtor(void *arg) {
  int produced, item, total=0;
  printf("Produtor criado\n");
  count_p=0;
//  for (produced = 0; produced < numIters; produced++) {
  while(1){
    item = rand()%10;
    //printf("PRODUTOR: Testando se o buffer ainda tem espaco....\n");
    //printf("PRODUTOR: Tem! Agora testando se pode entrar na regiao critica...\n");
    printf("PRODUTOR: Testando se pode entrar na regiao critica...\n");
    pthread_mutex_lock(&mutex);
    //printf("PRODUTOR: Pode!\n");
    printf("PRODUTOR: Pode! Agora testando se o buffer ainda tem espaco....\n");
    sem_wait(&empty);
    printf("++++++ PRODUTOR PRODUZINDO ++++++\n");
    data[count_p%BUFFER_SIZE] = item;
    total += item;
    count_p++;   
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
  }
  //printf("PRODUTOR: Por %d iteracoes, o total foi %d\n", numIters, total);
}

void *consumidor(void *arg) {
  int item, consumed, total=0;
  printf("Consumidor criado\n");
  count_c=0;
  //for (consumed = 0; consumed < numIters; consumed++) {
  while(1){
    printf("\tCONSUMIDOR: Testando se o buffer tem alguma coisa...\n");
    sem_wait(&full);
    printf("\tCONSUMIDOR: Tem! Agora testando se pode entrar na regiao critica...\n");
    pthread_mutex_lock(&mutex);
    printf("\t------ CONSUMIDOR CONSUMINDO ------\n");
    item = data[count_c%BUFFER_SIZE];
    total += item;
    count_c++;   
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
  }
  //printf("CONSUMIDOR: Por %d iteracoes, o total foi %d\n", numIters, total);
}

int main(int argc, char *argv[]) {
  pthread_t prod, cons;

  pthread_mutex_init(&mutex, NULL);  

  numIters = atoi(argv[1]);
  BUFFER_SIZE = atoi(argv[2]);
  data = (int*)malloc(BUFFER_SIZE*sizeof(int));
  sem_init(&empty, SHARED, atoi(argv[3]));  /* semaforo empty = 1 */
  sem_init(&full, SHARED, 0);   /* semaforo full = 0  */

  srand(time(NULL));

  printf("Inicio do Main\n");
  pthread_create(&prod, NULL, produtor, NULL);
  pthread_create(&cons, NULL, consumidor, NULL);
  pthread_join(prod, NULL);
  pthread_join(cons, NULL);

  pthread_mutex_destroy(&mutex);
 
  imprime_buffer();
  printf("Fim do Main\n");
}
