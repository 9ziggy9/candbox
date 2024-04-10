#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void ui_init(void) {
  initscr();
  curs_set(1);
}

int main(void) {
  ui_init();

  int screen_height = getmaxy(stdscr);
  int screen_width  = getmaxx(stdscr);

  WINDOW *w_out = newwin(screen_height - 4, screen_width - 2, 1, 1);
  assert(w_out != NULL && "uh oh newwin() failed.");
  box(w_out, 0, 0);
  mvwprintw(w_out, 1, 1, "Hello, world!");

  WINDOW *w_in = newwin(3, screen_width - 2, screen_height - 3, 1);
  box(w_in, 0, 0);
  mvwprintw(w_in, 1, 1, "> ");

  refresh();
  wrefresh(w_out);
  wrefresh(w_in);

  getch(); // this will block until a key is pressed

  delwin(w_out);
  endwin();
  return 0;
}
