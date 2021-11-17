.DEFAULT_GOAL := all

matrix.o: matrix.cpp matrix.h
	g++ matrix.cpp -c

reader.o: reader.cpp reader.h
	g++ reader.cpp -c	

objects: matrix.o reader.o

writer: objects writer.cpp
	g++ -Wall -fsanitize=address writer.cpp matrix.o reader.o -o writer

sequencial: objects sequencial.cpp
	g++ -Wall -fsanitize=address sequencial.cpp matrix.o reader.o -o sequencial	

thread: objects thread.cpp
	g++ -Wall -fsanitize=address thread.cpp matrix.o reader.o -o thread	

process: objects thread.cpp
	g++ -Wall -fsanitize=address process.cpp matrix.o reader.o -o process		

clean:
	rm *.o *.txt writer sequencial thread process

all: writer sequencial thread process
