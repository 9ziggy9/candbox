CC=clang
CFLAGS=-Wall -Wextra -pedantic -Wconversion \
			 -Wunreachable-code -Wswitch-enum -Wno-gnu
EXE=./i_should_exist

all: main

install: main
	cp $(EXE) $(HOME)/bin

main: len main.c
	$(CC) main.c -o $(EXE) $(CFLAGS) len.o

len: len.c
	$(CC) -c len.c -o len.o $(CFLAGS)

clean:
	rm -rf $(EXE)
