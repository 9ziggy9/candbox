# copy/paste code
# compile to object, and link
# header-only (define implementation)

CC=clang
CFLAGS=-Wall -Wextra -pedantic -Wconversion \
			 -Wunreachable-code -Wswitch-enum -Wno-gnu
EXE=./run

all: main

main: main.c
	$(CC) $(CFLAGS) main.c -o $(EXE) -lncurses

term:
	stty sane

clean:
	rm $(EXE) *.o *.i *.asm
