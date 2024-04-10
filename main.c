#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

void ui_init(void) {
  initscr();
  noecho();
  curs_set(1);
  set_escdelay(0); // immediate escape key response
}

#define LEN_INPUT_BUFFER 280

int main(void) {
  ui_init();

  int screen_height = getmaxy(stdscr);
  int screen_width  = getmaxx(stdscr);

  WINDOW *w_out = newwin(screen_height - 4, screen_width - 2, 1, 1);
  assert(w_out != NULL && "uh oh newwin() failed.");
  scrollok(w_out, TRUE);
  box(w_out, 0, 0);

  WINDOW *w_in = newwin(3, screen_width - 2, screen_height - 3, 1);
  box(w_in, 0, 0);
  mvwprintw(w_in, 1, 1, "> ");

  refresh();
  wrefresh(w_out);
  wrefresh(w_in);

  int ch;
  char input_buffer[LEN_INPUT_BUFFER] = {0};
  size_t cursor_position = 0;

  /*
    TODO: implement output_buffer which keeps a line number
    (analogous to cursor position) and stores input buffer output
    in a char[MAX_LINES][LEN_INPUT_BUFFER] (an array of strings)

    TODO: ncurses defines KEY_BACKSPACE, implement deleting of
    characters.
  */

  #define KEY_ESC 27
  while((ch = wgetch(w_in)) != KEY_ESC) {
    if (ch == '\n') {
      mvwprintw(w_out, 1, 1, "%s", input_buffer);
      memset(input_buffer, 0, LEN_INPUT_BUFFER);
      cursor_position = 0;
      wmove(w_in, 1, 3);
      wclrtoeol(w_in);
      wrefresh(w_in);
      wrefresh(w_out);
    } else if (cursor_position < LEN_INPUT_BUFFER) {
      input_buffer[cursor_position++] = (char) ch;
    }
    mvwprintw(w_in, 1, 3, "%s", input_buffer);
    wrefresh(w_in);
  }

  delwin(w_out);
  delwin(w_in);
  endwin();
  return 0;
}
