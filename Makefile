CC=clang
CFLAGS=-Wall -Wextra -pedantic -Wconversion \
			 -Wunreachable-code -Wswitch-enum -Wno-gnu
EXE=./run

pre: main.c
	$(CC) -E main.c -o main.i

asm: main.i
	$(CC) -S main.i -o main.asm

obj: main.asm
	$(CC) -c main.asm -o main.o

ld: main.o
	$(CC) main.o -o $(EXE)

clean:
	rm $(EXE)
