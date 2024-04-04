CC=clang
CFLAGS=-Wall -Wextra -pedantic -Wconversion \
			 -Wunreachable-code -Wswitch-enum -Wno-gnu
EXE=./run

all: main

install: main
	cp $(EXE) $(HOME)/bin/$(EXE)

main: len main.c
	$(CC) main.c -o $(EXE) $(CFLAGS) len.o

len: len.c
	$(CC) -c len.c -o len.o $(CFLAGS)

clean:
	rm -rf $(EXE)
