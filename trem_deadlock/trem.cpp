#include "trem.h"
#include <QtCore>
#include <QSemaphore>

#define N 5                  // Quantidade de trens
#define LEFT (i + N - 1) % N // Calculo do vizinho a esquerda de i
#define RIGHT (i + 1) % N    // Calculo do vizinho a direita de i
#define RUNNING 0            // Trem circulando
#define STOP 1               // Trem tentando entrar em um cruzamento
#define MOVING_CRITICAL 2    // Trem na região crítica

QSemaphore sem(N); // Um semáforo por trem

int state[N]; // Array de estados

//Construtor
Trem::Trem(int ID, int x, int y)
{
    this->ID = ID;
    this->x = x;
    this->y = y;
    velocidade = 100;
}

//Função a ser executada após executar trem->START
void Trem::run()
{
    while (true)
    {
        take_rail(ID);
        move(ID);
        put_rail(ID);
    }
}

void Trem::take_rail(int i)
{
    sem.acquire(i); // entra na região crítica
    state[i] = STOP;        // registra o desejo de continuar
    printf("O Trem %d deseja continuar.\n", i);
    test_rail(i);  // tenta entrar na região crítica
    sem.release(i);  // sai da regiao crítica
    //sem.acquire(i);  // sai da regiao crítica
}

void Trem::put_rail(int i)
{
    sem.acquire(i); // entra na região crítica
    state[i] = RUNNING;     // acabou de sair
    printf("O Trem %d saiu da região crítica.\n", i);
    sem.release(i); // sai da regiao crítica
}

void Trem::test_rail(int i)
{
    if (state[i] == STOP && state[LEFT] != MOVING_CRITICAL && state[RIGHT] != MOVING_CRITICAL)
    {
        state[i] = MOVING_CRITICAL;
        //printf("O Trem %d está na região crítica.\n", i);
        sem.release(i); // up
    }
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
