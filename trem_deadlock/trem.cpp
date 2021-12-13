#include "trem.h"
#include <QtCore>
#include <QMutex>
#include <QSemaphore>

#define N 5        // Quantidade de trens
#define INIT -1    // Trem circulando sem colisão
#define SAFE 0     // Trem circulando sem colisão
#define STOP 1     // Trem perto de uma zona crítica

#define TRAIN_0 0  // Trem circulando em zona de colisão
#define TRAIN_1 1  // Trem circulando em zona de colisão
#define TRAIN_2 2  // Trem circulando em zona de colisão
#define TRAIN_3 3  // Trem circulando em zona de colisão
#define TRAIN_4 4  // Trem circulando em zona de colisão

#define CROSSING_0 2   // Trem na circulando na região crítica #0
#define CROSSING_1 3   // Trem na circulando na região crítica #1
#define CROSSING_2 4   // Trem na circulando na região crítica #2
#define CROSSING_3 5   // Trem na circulando na região crítica #3
#define CROSSING_4 6   // Trem na circulando na região crítica #4
#define CROSSING_5 7   // Trem na circulando na região crítica #5
#define CROSSING_6 8   // Trem na circulando na região crítica #6
#define CROSSING_7 9   // Trem na circulando na região crítica #7

static QMutex mutex[6];
static QSemaphore sem[6];

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
    if(state[0] == INIT && state[1] == INIT && state[2] == INIT &&
            state[3] == INIT && state[4] == INIT) {

        sem[0].release(1);
        sem[1].release(2);
        sem[2].release(2);
        sem[3].release(2);
        sem[4].release(2);
        sem[5].release(1);
        sem[6].release(1);

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
        if(!this->parado) {
            switch (ID) {
            case 0: // Trem 1
                if(x == 420 && y == 50) { // DEADLOCK
                    sem[0].acquire();
                    sem[1].acquire();
                    mutex[0].lock();
                    mutex[1].lock();
                } else if(x == 370 && y == 160) { // #FREE #1
                    sem[0].release();
                    sem[1].release();
                    mutex[0].unlock();
                    mutex[1].unlock();
                } else {
                    move(ID);
                }
                break;
            case 1: // Trem 2
                if(x == 530 && y == 50) { // DEADLOCK #1
                    sem[5].acquire();
                    mutex[5].lock();
                }else if(x == 550 && y == 140) { // DEADLOCK #1
                    sem[4].acquire();
                    sem[2].acquire();
                    mutex[4].lock();
                    mutex[2].lock();
                } else if(x == 530 && y == 160) { // FREE #0
                    sem[5].release();
                    mutex[5].unlock();
                } else if(x == 460 && y == 160) { // DEADLOCK #1
                    sem[0].acquire();
                    mutex[0].lock();
                } else if(x == 440 && y == 140) { // FREE #0
                    sem[4].release();
                    sem[2].release();
                    mutex[4].unlock();
                    mutex[2].unlock();
                } else if(x == 460 && y == 50) { // FREE #0
                    sem[0].release();
                    mutex[0].unlock();
                } else {
                    move(ID);
                }

                break;
            case 2: // Trem 3
                if(x == 630 && y == 160) { // DEADLOCK #1
                    sem[5].acquire();
                    sem[6].acquire();
                    mutex[5].lock();
                    mutex[6].lock();
                }else if(x == 570 && y == 50) { // FREE #1
                    sem[5].release();
                    sem[6].release();
                    mutex[5].unlock();
                    mutex[6].unlock();
                } else {
                    move(ID);
                }
                break;
            case 3: // Trem 4
                if(x == 520 && y == 270) { // DEADLOCK #1
                    sem[3].acquire();
                    sem[4].acquire();
                    mutex[3].lock();
                    mutex[4].lock();
                }else if(x == 530 && y == 160) { // DEADLOCK #1
                    sem[6].acquire();
                    mutex[6].lock();
                } else if(x == 570 && y == 160) { // FREE #1
                    sem[3].release();
                    sem[4].release();
                    mutex[3].unlock();
                    mutex[4].unlock();
                } else if(x == 610 && y == 180) { // FREE #1
                    sem[6].release();
                    mutex[6].unlock();
                } else {
                    move(ID);
                }
                break;
            case 4: // Trem 5
                if(x == 390 && y == 180) { // DEADLOCK #1
                    sem[1].acquire();
                    sem[2].acquire();
                    mutex[1].lock();
                    mutex[2].lock();
                }else if(x == 480 && y == 160) { // DEADLOCK #7
                    sem[3].acquire();
                    mutex[3].lock();
                }else if(x == 500 && y == 150) { // FREE #2
                    sem[1].release();
                    sem[2].release();
                    mutex[1].unlock();
                    mutex[2].unlock();
                }else if(x == 480 && y == 270) { // FREE #
                    sem[3].release();
                    mutex[3].unlock();
                } else {
                    move(ID);
                }
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
