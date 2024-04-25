include custom.mk
CC=gcc
CFLAGS=-Wall -Wextra -pedantic -Wconversion -Wunreachable-code -Werror
EXE=./run

all: clean main
	$(call print_in_color,$(GREEN),\nRunning ...\n)
	@$(EXE)

main: main.c
	$(call print_in_color,$(YELLOW),\nCompiling to $(EXE) ...\n)
	$(CC) $(CFLAGS) main.c -o $(EXE)

clean:
	$(call print_in_color,$(RED),Cleaning ...\n)
	rm -rf $(EXE) *.o *.i *.asm
