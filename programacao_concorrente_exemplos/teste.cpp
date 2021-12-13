#include "trem.h"
#include <QtCore>
#include <QMutex>
#include <QSemaphore>

#define N 5        // Quantidade de trens
#define INIT -1    // Trem circulando sem colisão
#define RUNNING 0  // Trem circulando sem colisão
#define STOP 1     // Trem perto de uma zona crítica
#define CROSSING 2 // Trem na circulando na região crítica

static QSemaphore sem[6]; // Regiões críticas
static QMutex mutex[6];   // Zonas de colisões

int state[N] = {INIT, INIT, INIT, INIT, INIT}; // Array de estados

//Construtor
Trem::Trem(int ID, int x, int y)
{
  this->ID = ID;
  this->x = x;
  this->y = y;
  velocidade = 100;

  if (state[0] == INIT && state[1] == INIT && state[2] == INIT &&
      state[3] == INIT && state[4] == INIT)
  {
    // Instancia as zonas de colisões
    sem[0].release(1); // Zona de deadlock entre os trens 1, 2
    sem[1].release(1); // Zona de deadlock entre os trens 1, 4
    sem[2].release(1); // Zona de deadlock entre os trens 2, 4
    sem[3].release(1); // Zona de deadlock entre os trens 4, 5
    sem[4].release(1); // Zona de deadlock entre os trens 2, 5
    sem[5].release(1); // Zona de deadlock entre os trens 2, 3
    sem[5].release(1); // Zona de deadlock entre os trens 3, 5

    // Intancia os valores dos trens
    state[0] = RUNNING;
    state[1] = RUNNING;
    state[2] = RUNNING;
    state[3] = RUNNING;
    state[4] = RUNNING;
  }
}

//Função a ser executada após executar trem->START
void Trem::run()
{
  while (true)
  {
    //printf("Status do Trem#%d é %d.\n",ID, state[ID]);

    switch (ID)
    {
    case 0: // Trem 1
      if (x == 420 && y == 50)
      {                  // Zona critica
        mutex[0].lock(); // Entra na regiao critica
        mutex[1].lock(); // Entra na regiao critica

        printf("Alterando o status do Trem#%d para STOP.\n", ID);

        state[ID] = STOP; // Altera o status para STOP

        // Verifica o status do trem 2 e trem 5
        if (state[ID] == STOP && state[1] != CROSSING && state[4] != CROSSING)
        {
          state[ID] = CROSSING;
          printf("Alterando o status do Trem#%d para CROSSING.\n", ID);
        }

        sem[0].acquire();
        sem[1].acquire();
      }

      if (x == 370 && y == 160)
      { // Zona segura
        mutex[0].unlock();
        mutex[1].unlock();

        state[ID] = RUNNING; // Altera o status para RUNNING

        sem[0].release();
        sem[1].release();
      }

      if (state[ID] != STOP)
      {
        move(ID);
      }
      break;
    case 1: // Trem 2
      if (x == 530 && y == 50)
      {                   // Zona critica
        mutex[5].lock();  // Entra na regiao critica
        state[ID] = STOP; // Altera o status para STOP

        // Verifica o status do trem 3
        if (state[ID] == STOP && state[2] != CROSSING)
        {
          state[ID] = CROSSING;
          //printf("Alterando o status do Trem#%d para CROSSING.\n", ID);
        }

        sem[5].acquire();
      }

      if (x == 550 && y == 140)
      {                   // Zona critica
        mutex[4].lock();  // Entra na regiao critica
        mutex[2].lock();  // Entra na regiao critica
        state[ID] = STOP; // Altera o status para STOP

        // Verifica o status do trem 4 e trem 5
        if (state[ID] == STOP && state[3] != CROSSING && state[4] != CROSSING)
        {
          state[ID] = CROSSING;
        }

        sem[4].acquire();
        sem[2].acquire();
      }

      if (x == 530 && y == 160)
      {                    // Zona segura
        mutex[5].unlock(); // Libera a regiao critica 5
        sem[5].release();  // Libera a região 5
      }

      if (x == 460 && y == 160)
      {                   // Zona critica
        mutex[0].lock();  // Entra na regiao critica
        state[ID] = STOP; // Altera o status para STOP

        printf("Alterando o status do Trem#%d para STOP.\n", ID);
        printf("Alterando o status do Trem#0 e %d.\n", state[0]);

        // Verifica o status do trem 1
        if (state[ID] == STOP && state[0] != CROSSING)
        {
          state[ID] = CROSSING;
        }

        sem[0].acquire();
      }

      if (x == 440 && y == 140)
      {                    // Zona segura
        mutex[4].unlock(); // Libera a regiao critica 4
        mutex[2].unlock(); // Libera a regiao critica 2
        sem[4].release();  // Libera a região 4
        sem[2].release();  // Libera a região 2
      }

      if (x == 460 && y == 50)
      {                      // Zona segura
        mutex[0].unlock();   // Entra na regiao critica
        state[ID] = RUNNING; // Altera o status para STOP
        sem[0].release();    // Libera a região 4
      }

      if (state[ID] != STOP)
      {
        move(ID);
        //printf("Trem#%d rodando.\n", ID);
      }
      break;
    case 2: // Trem 3
      if (x == 630 && y == 160)
      { // Zona critica
        mutex[6].lock();
        state[ID] = STOP;

        // Verifica o status do trem 4
        if (state[ID] == STOP && state[3] != CROSSING)
        {
          state[ID] = CROSSING;
        }

        sem[6].acquire();
      }

      if (x == 570 && y == 160)
      { // Zona critica
        mutex[5].lock();
        state[ID] = STOP;

        // Verifica o status do trem 3
        if (state[ID] == STOP && state[2] != CROSSING)
        {
          state[ID] = CROSSING;
        }

        sem[5].acquire();
      }

      if (x == 550 && y == 140)
      { // Zona segura
        mutex[6].unlock();
        sem[6].release();
      }

      if (x == 570 && y == 50)
      { // Zona segura
        mutex[5].unlock();
        state[ID] = RUNNING; // Altera o status para STOP
        sem[5].release();
      }

      if (state[ID] != STOP)
      {
        move(ID);
      }
      break;
    case 3: // Trem 4
      if (x == 520 && y == 270)
      { // Zona critica
        mutex[3].lock();
        state[ID] = STOP;

        // Verifica o status do trem 5
        if (state[ID] == STOP && state[4] != CROSSING)
        {
          state[ID] = CROSSING;
        }

        sem[3].acquire();
      }

      if (x == 500 && y == 180)
      { // Zona critica
        mutex[4].lock();
        mutex[6].lock();

        state[ID] = STOP;

        // Verifica o status do trem 2 e trem 3
        if (state[ID] == STOP && state[1] != CROSSING && state[2] != CROSSING)
        {
          state[ID] = CROSSING;
        }

        sem[4].acquire();
        sem[6].acquire();
      }

      if (x == 520 && y == 160)
      { // Zona segura
        mutex[3].unlock();
        sem[3].release();
      }

      if (x == 610 && y == 180)
      { // Zona segura
        mutex[4].unlock();
        mutex[6].unlock();
        state[ID] = RUNNING;
        sem[4].release();
        sem[6].release();
      }

      if (state[ID] != STOP)
      {
        move(ID);
      }
      break;
    case 4: // Trem 5
      if (x == 390 && y == 160)
      { // Zona critica
        mutex[1].lock();
        mutex[2].lock();

        state[ID] = STOP;

        // Verifica o status do trem 1 e trem 2
        if (state[ID] == STOP && state[0] != CROSSING && state[1] != CROSSING)
        {
          state[ID] = CROSSING;
        }

        sem[1].acquire();
        sem[2].acquire();
      }

      if (x == 480 && y == 160)
      { // Zona critica
        mutex[3].lock();
        state[ID] = STOP;

        // Verifica o status do trem 4
        if (state[ID] == STOP && state[3] != CROSSING)
        {
          state[ID] = CROSSING;
        }

        sem[3].acquire();
      }

      if (x == 500 && y == 180)
      { // Zona segura
        mutex[1].unlock();
        mutex[2].unlock();
        sem[1].release();
        sem[2].release();
      }

      if (x == 480 && y == 270)
      { // Zona segura
        mutex[3].unlock();
        state[ID] = RUNNING;
        sem[3].release();
      }

      if (state[ID] != STOP)
      {
        move(ID);
      }
      break;
    default:
      break;
    }
  }
}

void Trem::move(int ID)
{
  switch (ID)
  {
  case 0: // Trem 1 (330,50) (440, 150)
    if (y == 50 && x < 440)
      x += 10;
    else if (x == 440 && y < 160)
      y += 10;
    else if (x > 330 && y == 160)
      x -= 10;
    else
      y -= 10;
    emit updateGUI(ID, x, y); //Emite um sinal
    break;
  case 1: // Trem 2 (540, 50) (450, 160)
    if (y == 50 && x < 550)
      x += 10;
    else if (x == 550 && y < 160)
      y += 10;
    else if (x > 440 && y == 160)
      x -= 10;
    else
      y -= 10;
    emit updateGUI(ID, x, y); //Emite um sinal
    break;
  case 2: // Trem 3 (620, 160) (560, 50)
    if (y == 50 && x < 660)
      x += 10;
    else if (x == 660 && y < 160)
      y += 10;
    else if (x > 550 && y == 160)
      x -= 10;
    else
      y -= 10;
    emit updateGUI(ID, x, y); //Emite um sinal
    break;
  case 3: //Trem 4 (510, 270) (610, 170)
    if (y == 160 && x < 610)
      x += 10;
    else if (x == 610 && y < 270)
      y += 10;
    else if (x > 500 && y == 270)
      x -= 10;
    else
      y -= 10;
    emit updateGUI(ID, x, y); //Emite um sinal
    break;
  case 4: //Trem 5 (390, 170) (490, 170)
    if (y == 160 && x < 500)
      x += 10;
    else if (x == 500 && y < 270)
      y += 10;
    else if (x > 390 && y == 270)
      x -= 10;
    else
      y -= 10;
    emit updateGUI(ID, x, y); //Emite um sinal
    break;
  default:
    break;
  }
  msleep(velocidade);
}
