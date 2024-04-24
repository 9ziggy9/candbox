#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

void exit_handler(int exit_code, void *args) {
  (void) args;
  if (exit_code == EXIT_FAILURE) printf("EXIT FAILURE\n");
}

void i_always_fail(void) {
  exit(EXIT_FAILURE);
}

int main(void) {
  on_exit(exit_handler, NULL);
  initscr();

_loop:
  getch();
  printf("Hello in loop!");
  goto _loop;

  return 0;
}
