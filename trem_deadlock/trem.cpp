#include "trem.h"
#include <QtCore>
#include <QMutex>
#include <QSemaphore>

static QMutex mutex[6]; // 1 para cada região de colisão entre 2 trens
static QSemaphore mutualCritical[6]; // Semáforos para controlar as regiões de deadlock

//Construtor
Trem::Trem(int ID, int x, int y)
{
  this->ID = ID;
  this->x = x;
  this->y = y;
  velocidade = 100;
  parado = false;

  if (ID == 1) {
          mutualCritical[0].release(2); // 01
          mutualCritical[1].release(2); // 04
          mutualCritical[2].release(2); // 14
          mutualCritical[3].release(2); // 34
          mutualCritical[4].release(2); // 13
          mutualCritical[5].release(2); // 12
          mutualCritical[6].release(2); // 32
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
            mutualCritical[0].acquire();
            mutex[0].lock();
        }

        if (x == 440 && y == 140)
        { // DEADLOCK
            mutualCritical[1].acquire();
            mutex[1].lock();
        }

        if (x == 420 && y == 160)
        { // FREE
            mutex[0].unlock();
            mutualCritical[0].release();
        }

        if (x == 370 && y == 160)
        { // FREE
            mutex[1].unlock();
            mutualCritical[1].release();
        }

        move(ID);
        break;
      case 1: // Trem 2
//        if(x == 530 && y == 50)
//        {

//        }

//        if(x == 550 && y == 140)
//        {

//        }

//        if(x == 530 && y == 160)
//        {

//        }

//        if(x == 520 && y == 160)
//        {

//        }

//        if(x == 480 && y == 160)
//        {

//        }

//        if(x == 460 && y == 160)
//        {

//        }

//        if(x == 440 && y == 140)
//        {

//        }

//        if(x == 460 && y == 50)
//        {

//        }
        move(ID);
        break;
      case 2: // Trem 2
        if(x == 630 && y == 160)
        {
            mutualCritical[6].acquire();
            mutex[6].lock();
        }

        if(x == 570 && y == 160)
        {
            mutualCritical[5].acquire();
            mutex[5].lock();
        }

        if(x == 550 && y == 140)
        {
            mutex[6].unlock();
            mutualCritical[6].release();
        }

        if(x == 570 && y == 50)
        {
            mutex[5].unlock();
            mutualCritical[5].release();
        }

        move(ID);
        break;
      case 3: // Trem 4
        if(x == 520 && y == 270)
        {
            mutualCritical[3].acquire();
            mutex[3].lock();
        }

        if(x == 500 && y == 180)
        {
            mutualCritical[4].acquire();
            mutex[4].lock();
        }

        if(x == 520 && y == 160)
        {
            mutex[3].unlock();
            mutualCritical[3].release();
        }

        if(x == 530 && y == 160)
        {
            mutualCritical[6].acquire();
            mutex[6].lock();
        }

        if(x == 570 && y == 160)
        {
            mutex[4].unlock();
            mutualCritical[4].release();
        }

        if(x == 610 && y == 180)
        {
            mutex[6].unlock();
            mutualCritical[6].release();
        }
        move(ID);
        break;
      case 4: // Trem 5
        if(x == 390 && y == 180)
        {
            mutualCritical[1].acquire();
            mutex[1].lock();
        }

        if(x == 420 && y == 160)
        {
            mutex[2].lock();
            mutualCritical[2].acquire();
        }


        if(x == 460 && y == 160)
        {
            mutex[1].unlock();
            mutualCritical[1].release();
        }

        if(x == 480 && y == 160)
        {
            mutex[3].lock();
            mutualCritical[3].acquire();
        }

        if(x == 500 && y == 180)
        {
            mutex[2].unlock();
            mutualCritical[2].release();
        }

        if(x == 480 && y == 270)
        {
            mutex[3].unlock();
            mutualCritical[3].release();
        }

        move(ID);
        break;
      default:
        break;
      }
    }
  }
}

void Trem::alterar_velocidade(int value){
    this->velocidade = value;
}

void Trem::parar(bool value) {
    this->parado = value;
}

void Trem::move(int ID) {
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
