CC=clang
CFLAGS=-Wall -Wextra -pedantic -Wconversion \
			 -Wunreachable-code -Wswitch-enum -Wno-gnu \
			 -ggdb
EXE=./calcu
LIBS=-lncurses -lm

all: main

parse: parse_test.c
	$(CC) $(CFLAGS) $< -o ./test $(LIBS)

main: main.c
	$(CC) $(CFLAGS) main.c -o $(EXE) $(LIBS)

clean:
	rm -rf $(EXE) *.o *.i
