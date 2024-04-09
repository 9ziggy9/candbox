CC=clang
CFLAGS=-Wall -Wextra -pedantic -Wconversion \
			 -Wunreachable-code -Wswitch-enum -Wno-gnu \
			 -ggdb
EXE=./calcu
LIBS=-lncurses -lm

all: main

main: main.c
	$(CC) $(CFLAGS) main.c -o $(EXE) $(LIBS)

clean:
	rm -rf $(EXE) *.o *.i
