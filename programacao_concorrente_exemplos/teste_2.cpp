#include "trem.h"
#include <QtCore>
#include <QMutex>
#include <QSemaphore>

#define N 5     // Quantidade de trens
#define INIT -1 // Trem circulando sem colisão
#define SAFE 0  // Trem circulando sem colisão
#define STOP 1  // Trem perto de uma zona crítica

#define TRAIN_0 0 // Trem circulando em zona de colisão
#define TRAIN_1 1 // Trem circulando em zona de colisão
#define TRAIN_2 2 // Trem circulando em zona de colisão
#define TRAIN_3 3 // Trem circulando em zona de colisão
#define TRAIN_4 4 // Trem circulando em zona de colisão

#define CROSSING_0 2 // Trem na circulando na região crítica #0
#define CROSSING_1 3 // Trem na circulando na região crítica #1
#define CROSSING_2 4 // Trem na circulando na região crítica #2
#define CROSSING_3 5 // Trem na circulando na região crítica #3
#define CROSSING_4 6 // Trem na circulando na região crítica #4
#define CROSSING_5 7 // Trem na circulando na região crítica #5
#define CROSSING_6 8 // Trem na circulando na região crítica #6
#define CROSSING_7 9 // Trem na circulando na região crítica #7

static QMutex mutex[6];
static QSemaphore s[6];

int state[N] = {INIT, INIT, INIT, INIT, INIT}; // Array de estados

//Construtor
Trem::Trem(int ID, int x, int y)
{
  this->ID = ID;
  this->x = x;
  this->y = y;
  velocidade = 100;
  parado = false;

  // Instancia as zonas de colisões
  if (state[0] == INIT && state[1] == INIT && state[2] == INIT &&
      state[3] == INIT && state[4] == INIT)
  {

    mutualCritical[0].release(1);
    mutualCritical[1].release(2);
    mutualCritical[2].release(3);
    mutualCritical[3].release(3);
    mutualCritical[4].release(3);
    mutualCritical[5].release(2);
    mutualCritical[6].release(2);

    // Intancia os valores dos trens
    state[0] = SAFE;
    state[1] = SAFE;
    state[2] = SAFE;
    state[3] = SAFE;
    state[4] = SAFE;
  }
}

//Função a ser executada após executar trem->START
void Trem::run()
{
  while (true)
  {
    if (!this->parado)
    {
      switch (ID)
      {
      case 0: // Trem 1
        if (x == 420 && y == 50)
        { // DEADLOCK
          take_rail(ID, TRAIN_1, CROSSING_0);
        }

        if (x == 440 && y == 140)
        { // DEADLOCK
          take_rail(ID, TRAIN_4, CROSSING_1);
        }

        //                if(x == 370 && y == 160) { // FREE #0

        //                }

        if (x == 370 && y == 160)
        { // #FREE #1
          state[ID] = SAFE;
          put_rail(ID, TRAIN_4, CROSSING_1);
          put_rail(ID, TRAIN_0, CROSSING_0);
        }

        if (state[ID] != STOP)
        {
          move(ID);
        }
        break;
      case 1: // Trem 2
        if (x == 530 && y == 50)
        { // DEADLOCK #1
          take_rail(ID, TRAIN_2, CROSSING_5);
        }

        if (x == 550 && y == 140)
        { // DEADLOCK #1
          take_rail(ID, TRAIN_3, CROSSING_4);
        }

        //                if(x == 530 && y == 160) { // FREE #0
        //                    put_rail(ID, TRAIN_2, CROSSING_5);
        //                }

        if (x == 520 && y == 160)
        { // DEADLOCK #1
          take_rail(ID, TRAIN_4, CROSSING_2);
        }

        if (x == 480 && y == 160)
        { // FREE #0
          put_rail(ID, TRAIN_2, CROSSING_5);
        }

        if (x == 460 && y == 160)
        { // DEADLOCK #1
          take_rail(ID, TRAIN_0, CROSSING_0);
        }

        if (x == 440 && y == 140)
        { // FREE #0
          put_rail(ID, TRAIN_3, CROSSING_4);
          put_rail(ID, TRAIN_4, CROSSING_2);
        }

        if (x == 460 && y == 50)
        { // FREE #0
          state[ID] = SAFE;
          put_rail(ID, TRAIN_0, CROSSING_0);
          put_rail(ID, TRAIN_4, CROSSING_2);
        }

        if (state[ID] != STOP)
        {
          move(ID);
        }

        break;
      case 2: // Trem 3
        if (x == 630 && y == 160)
        { // DEADLOCK #1
          take_rail(ID, TRAIN_3, CROSSING_6);
        }

        if (x == 570 && y == 160)
        { // DEADLOCK #1
          take_rail(ID, TRAIN_1, CROSSING_5);
        }

        //                if(x == 550 && y == 140) { // FREE #0
        //                    put_rail(ID, TRAIN_3, CROSSING_6);
        //                }

        if (x == 570 && y == 50)
        { // FREE #1
          state[ID] = SAFE;
          put_rail(ID, TRAIN_1, CROSSING_5);
          put_rail(ID, TRAIN_3, CROSSING_6);
        }

        if (state[ID] != STOP)
        {
          move(ID);
        }
        break;
      case 3: // Trem 4
        if (x == 520 && y == 270)
        { // DEADLOCK #1
          take_rail(ID, TRAIN_4, CROSSING_7);
        }

        if (x == 500 && y == 180)
        { // DEADLOCK #1
          take_rail(ID, TRAIN_1, CROSSING_4);
        }

        //                if(x == 520 && y == 160) { // FREE #1
        //                    put_rail(ID, TRAIN_4, CROSSING_7);
        //                }

        if (x == 530 && y == 160)
        { // DEADLOCK #1
          take_rail(ID, TRAIN_2, CROSSING_6);
        }

        if (x == 570 && y == 160)
        { // FREE #1
          put_rail(ID, TRAIN_1, CROSSING_4);
          put_rail(ID, TRAIN_4, CROSSING_7);
        }

        if (x == 610 && y == 180)
        { // FREE #1
          state[ID] = SAFE;
          put_rail(ID, 2, CROSSING_6);
        }

        if (state[ID] != STOP)
        {
          move(ID);
        }
        break;
      case 4: // Trem 5
        if (x == 390 && y == 180)
        { // DEADLOCK #1
          take_rail(ID, TRAIN_0, CROSSING_1);
        }

        if (x == 420 && y == 160)
        { // DEADLOCK #2
          take_rail(ID, TRAIN_1, CROSSING_2);
        }

        //                if(x == 460 && y == 160) { // FREE #1
        //                    put_rail(ID, TRAIN_0, CROSSING_1);
        //                }

        if (x == 480 && y == 160)
        { // DEADLOCK #7
          take_rail(ID, TRAIN_3, CROSSING_7);
        }

        if (x == 500 && y == 150)
        { // FREE #2
          put_rail(ID, TRAIN_1, CROSSING_2);
          put_rail(ID, TRAIN_0, CROSSING_1);
        }

        if (x == 480 && y == 270)
        { // FREE #
          state[ID] = SAFE;
          put_rail(ID, TRAIN_3, CROSSING_7);
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
}

void Trem::take_rail(int my_train, int other_train, int state_other_train)
{
  sem_wait(&mutex);
  state[my_train] = STOP;
  test_rail(my_train, other_train, state_other_train);
  sem_post(&mutex);       //up(&mutex) sai da regiao cri­tica
  sem_wait(&s[my_train]); //down(&s[i])
}

void Trem::put_rail(int my_train, int other_train, int state_critical)
{
  sem_wait(&mutex); //down(&mutex) entra na regiao critica

  if (state[other_train] == STOP && state[my_train] != state_critical)
  {
    state[other_train] = state_critical;
    sem_post(&s[other_train]); //up(&s[i]);
  }

  sem_post(&mutex); //up(&mutex);  /* sai da regiao cri­tica */
}

void Trem::test_rail(int my_train, int other_train, int state_other_train)
{
  if (state[my_train] == STOP && state[other_train] != state_other_train)
  {
    state[my_train] = state_other_train;
    sem_post(&s[my_train]); //up(&s[i]);
  }
}

void Trem::alterar_velocidade(int value)
{
  this->velocidade = value;
}

void Trem::parar(bool value)
{
  this->parado = value;
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
