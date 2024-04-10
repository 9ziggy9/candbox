# copy/paste code
# compile to object, and link
# header-only (define implementation)

CC=clang
CFLAGS=-Wall -Wextra -pedantic -Wconversion \
			 -Wunreachable-code -Wswitch-enum -Wno-gnu
EXE=./run

all: lib.o main

main: main.c
	$(CC) $(CFLAGS) main.c -o $(EXE) lib.o

lib.o: lib.c
	$(CC) $(CFLAGS) -c lib.c -o lib.o

# pre: main.c
# 	$(CC) -E main.c -o main.i

# asm: main.i
# 	$(CC) -S main.i -o main.asm

# obj: main.asm
# 	$(CC) -c main.asm -o main.o

# ld: main.o
# 	$(CC) main.o -o $(EXE)

clean:
	rm $(EXE)
