.DEFAULT_GOAL := all

matrix.o: matrix.cpp matrix.h
	g++ matrix.cpp -c

objects: matrix.o

generator: objects generator.cpp
	g++ -Wall -fsanitize=address generator.cpp matrix.o -o generator

sequencial: objects sequencial.cpp
	g++ -Wall -fsanitize=address sequencial.cpp matrix.o -o sequencial	

clean:
	rm *.o matrix_a.txt matrix_b.txt sequencial_result.txt generator sequencial

all: generator sequencial
