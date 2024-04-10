#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

void ui_init(void) {
  initscr();
  noecho();
  curs_set(1);
}

#define LEN_INPUT_BUFFER 280

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

  int ch;
  char input_buffer[LEN_INPUT_BUFFER] = {0};
  size_t cursor_position = 0;

  while((ch = wgetch(w_in)) != 'q') {
    if (cursor_position < LEN_INPUT_BUFFER) {
      input_buffer[cursor_position++] = ch;
    }
    mvwprintw(w_in, 1, 3, "%s", input_buffer);
    wrefresh(w_in);
  }

  delwin(w_out);
  delwin(w_in);
  endwin();
  return 0;
}
