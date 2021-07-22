TARGET: main

all: main.o Comand.o list.o
	gcc main.o Comand.o list.o -o main

main.o: main.c
	gcc -c main.c -o main.o

Comand.o: Comand.c Comand.h
	gcc -c Comand.c -o Comand.o

list.o: list.c list.h
	gcc -c list.c -o list.o

clean:
	rm *.o ./main
