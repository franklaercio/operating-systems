#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<string>

#define RUNNING 0       // Trem circulando sem colisão

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Cria o trem com seu (ID, posição X, posição Y)
    trem1 = new Trem(0,330,100);
    trem2 = new Trem(1,500,50);
    trem3 = new Trem(2,610,50);
    trem4 = new Trem(3,610,210);
    trem5 = new Trem(4,450,270);

    /*
     * Conecta o sinal UPDATEGUI à função UPDATEINTERFACE.
     * Ou seja, sempre que o sinal UPDATEGUI foi chamado, será executada a função UPDATEINTERFACE.
     * Os 3 parâmetros INT do sinal serão utilizados na função.
     * Trem1 e Trem2 são os objetos que podem chamar o sinal. Se um outro objeto chamar o
     * sinal UPDATEGUI, não haverá execução da função UPDATEINTERFACE
     */
    connect(trem1,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem2,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem3,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem4,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem5,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));

    trem1->start();
    trem2->start();
    trem3->start();
    trem4->start();
    trem5->start();
}

//Função que será executada quando o sinal UPDATEGUI for emitido
void MainWindow::updateInterface(int id, int x, int y){
    switch(id){
    case 0: //Atualiza a posição do objeto da tela (quadrado) que representa o trem1
        ui->label_trem1->setGeometry(x,y,10,10);
        break;
    case 1: //Atualiza a posição do objeto da tela (quadrado) que representa o trem2
        ui->label_trem2->setGeometry(x,y,10,10);
        break;
    case 2: //Atualiza a posição do objeto da tela (quadrado) que representa o trem2
        ui->label_trem3->setGeometry(x,y,10,10);
        break;
    case 3: //Atualiza a posição do objeto da tela (quadrado) que representa o trem2
        ui->label_trem4->setGeometry(x,y,10,10);
        break;
    case 4: //Atualiza a posição do objeto da tela (quadrado) que representa o trem2
        ui->label_trem5->setGeometry(x,y,10,10);
        break;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_slider_0_sliderReleased()
{
    int vel = 200-ui->slider_0->value();
    trem1->alterar_velocidade(vel);
    trem1->parar(vel == 200);
}

void MainWindow::on_slider_1_sliderReleased()
{
    int vel = 200-ui->slider_1->value();
    trem2->alterar_velocidade(vel);
    trem2->parar(vel == 200);
}

void MainWindow::on_slider_2_sliderReleased()
{
    int vel = 200-ui->slider_2->value();
    trem3->alterar_velocidade(vel);
    trem3->parar(vel == 200);
}

void MainWindow::on_slider_3_sliderReleased()
{
    int vel = 200-ui->slider_3->value();
    trem4->alterar_velocidade(vel);
    trem4->parar(vel == 200);
}

void MainWindow::on_slider_4_sliderReleased()
{
    int vel = 200-ui->slider_4->value();
    trem5->alterar_velocidade(vel);
    trem5->parar(vel == 200);
}
