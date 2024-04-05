CC=clang
CFLAGS=-Wall -Wextra -pedantic -Wconversion \
			 -Wunreachable-code -Wswitch-enum -Wno-gnu \
			 -ggdb
EXE=./run

all: main

pre: main.c
	$(CC) -E main.c -o main.i

install: main
	cp $(EXE) $(HOME)/bin

main: len main.c
	$(CC) main.c -o $(EXE) $(CFLAGS) len.o

len: len.c
	$(CC) -c len.c -o len.o $(CFLAGS)

clean:
	rm -rf $(EXE) *.o
