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

main: main.c
	$(CC) main.c -o $(EXE) $(CFLAGS)

test: test.c
	$(CC) test.c -o ./test $(CFLAGS)

clean:
	rm -rf $(EXE) ./test *.o *.i

git:
	git add . && git commit -m "automated push" && git push
