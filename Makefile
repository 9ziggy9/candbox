CC=gcc
CFLAGS=-Wall -Wextra -pedantic -Wconversion -Wunreachable-code -Werror
EXE=./run

all: clean main
	$(EXE)

main: main.c
	$(CC) $(CFLAGS) main.c -o $(EXE)

clean:
	rm -rf $(EXE) *.o *.i *.asm
