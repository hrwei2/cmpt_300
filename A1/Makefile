## This is a simple Makefile

# Define what compiler to use and the flags.
CC=cc
CCFLAGS= -g -std=c99 -Wall -Werror


final: main.o list.o
	$(CC) $(CCFLAGS) -o final main.o list.o


list: list.c list.h
	$(CC) $(CCFLAGS) -c list.c

main.o: main.c list.h
	$(CC) $(CCFLAGS) -c main.c
clean:
	$(RM) final *.o *~
