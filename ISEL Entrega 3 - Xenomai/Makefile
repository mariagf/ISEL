CC=gcc
CFLAGS=-g -Wall -O -I.-DNDEBUG
LDFLAGS=
LDLIBS=-lrt -lpthread

all: main

main: main.o tasks.o fsm.o

clean:
	$(RM) *.o *~ main
