.DEFAULT_GOAL := all

matrix.o: matrix.cpp matrix.h
	g++ matrix.cpp -c

objects: matrix.o

generator: objects generator.cpp
	g++ -Wall -fsanitize=address generator.cpp matrix.o -o generator

clean:
	rm *.o matrix_a.txt matrix_b.txt generator

all: generator
