#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void clear_input(WINDOW *);

void ui_init(void) {
  initscr();
  noecho();
  curs_set(1);
  set_escdelay(0); // immediate escape key response
}

#define EXIT_WIN_ALLOC_FAIL 1000

void cleanup_ncurses(int exit_code, void *args) {
  WINDOW **ws = (WINDOW **) args;
  if (exit_code == EXIT_WIN_ALLOC_FAIL) {
    fprintf(stderr, "ERROR: allocation of window failed\n");
  }
  delwin(ws[0]); // output
  delwin(ws[1]); // input
  endwin();
}

#define LEN_INPUT_BUFFER 280

int main(void) {
  WINDOW *w_out = NULL, *w_in = NULL;
  ui_init();
  on_exit(cleanup_ncurses, (WINDOW *[]){w_out, w_in});

  int screen_height = getmaxy(stdscr);
  int screen_width  = getmaxx(stdscr);

  w_out = newwin(screen_height - 4, screen_width - 2, 1, 1);
  if (w_out == NULL) exit(EXIT_WIN_ALLOC_FAIL);
  scrollok(w_out, TRUE);
  box(w_out, 0, 0);

  w_in = newwin(3, screen_width - 2, screen_height - 3, 1);
  if (w_in == NULL) exit(EXIT_WIN_ALLOC_FAIL);
  keypad(w_in, TRUE);
  box(w_in, 0, 0);
  mvwprintw(w_in, 1, 1, "> ");

  refresh();
  wrefresh(w_out);
  wrefresh(w_in);

  int ch;
  char input_buffer[LEN_INPUT_BUFFER] = {0};
  size_t cursor_p = 0;

  /* TODO: implement output_buffer which keeps a line number
    (analogous to cursor p) and stores input buffer output
    in a char[MAX_LINES][LEN_INPUT_BUFFER] (an array of strings)

    TODO: ncurses defines KEY_BACKSPACE, implement deleting of
    characters. */

  #define KEY_ESC 27
  while((ch = wgetch(w_in)) != KEY_ESC) {
    if (ch == '\n') {
      mvwprintw(w_out, 1, 1, "%s", input_buffer);
      memset(input_buffer, 0, LEN_INPUT_BUFFER);
      cursor_p = 0;
      clear_input(w_in);
      wrefresh(w_in);
      wrefresh(w_out);
    } else if (cursor_p < LEN_INPUT_BUFFER && cursor_p >= 0) {
      if (ch == KEY_BACKSPACE) {
        input_buffer[cursor_p] = '\0';
        cursor_p = cursor_p == 0 ? cursor_p : cursor_p - 1;
      } else {
        input_buffer[cursor_p++] = (char) ch;
      }
    }
    clear_input(w_in);
    mvwprintw(w_in, 1, 3, "%s", input_buffer);
    wrefresh(w_in);
  }

  return EXIT_SUCCESS;
}

void clear_input(WINDOW *w_in) {
  wmove(w_in, 1, 3);
  wclrtoeol(w_in);
  box(w_in, 0, 0);
}
