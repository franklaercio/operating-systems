<h2 align="center"> 
  📈 Matrix Multiply
</h1>

### Resumo

Este relatório é resultado do desenvolvimento do “Trabalho da Unidade 1”, que busca aplicar na prática parte dos conteúdos vistos nas aulas durante a primeira unidade. <br />
Logo, a proposta foi desenvolver um programa que multiplicasse matrizes quadráticas em três maneiras diferentes: sequencial, thread(s) e processo(s). 

- C++
- Makefile
- Thread
- Process

### Como obter o projeto e executar o código

Para obter o projeto é bem simples, basta apenas executar os comandos abaixo:

```
# Clone este repositório
$ git clone https://github.com/franklaercio/operating-systems.git

# Acesse a pasta do projeto
$ cd operating-systems/matrix-sequential-thread-processes

# Compilar os arquivos
$ make all

# Escrever as matrizes m1 e m2
$ ./writer 100 100 100 100

# Executar programa sequencial
$ ./sequencial

# Executar programa em threads
$ ./threads 5

# Executar programa em processos
$ ./process 5
```
