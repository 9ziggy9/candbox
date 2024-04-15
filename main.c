#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void clear_input(WINDOW *);
void clear_output(WINDOW *);

void ui_init(void) {
  initscr();
  noecho();
  curs_set(1);
  set_escdelay(0); // immediate escape key response
  start_color();
}

#define EXIT_WIN_ALLOC_FAIL 1000
#define EXIT_OUT_OVERFLOW 1001

void cleanup_ncurses(int exit_code, void *args) {
  WINDOW **ws = (WINDOW **) args;
  delwin(ws[0]); // output
  delwin(ws[1]); // input
  endwin();
  if (exit_code == EXIT_WIN_ALLOC_FAIL) {
    fprintf(stderr, "ERROR: allocation of window failed\n");
  }
  if (exit_code == EXIT_OUT_OVERFLOW) {
    fprintf(stderr, "ERROR: overflew out window.\n");
  }
}

#define LEN_INPUT_BUFFER 280

#ifndef PARSE_TEST
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


  #define MAX_OUT_LINES 20
  int ch;
  char input_buffer[LEN_INPUT_BUFFER] = {0};
  char output_buffer[MAX_OUT_LINES][LEN_INPUT_BUFFER] = {0};
  size_t cursor_p = 0;
  size_t line_p = 0;

  /* TODO: implement output_buffer which keeps a line number
    (analogous to cursor p) and stores input buffer output
    in a char[MAX_LINES][LEN_INPUT_BUFFER] (an array of strings) */

  #define KEY_ESC 27
  while((ch = wgetch(w_in)) != KEY_ESC) {
    if (ch == '\n') {
      if (line_p > MAX_OUT_LINES) exit(EXIT_OUT_OVERFLOW);
      clear_output(w_out);
      memcpy(output_buffer[line_p++], input_buffer, LEN_INPUT_BUFFER);
      for (size_t y = 0; y < line_p; y++) {
        init_pair(4, COLOR_RED, COLOR_BLACK);
        wattron(w_out, COLOR_PAIR(4));
        mvwprintw(w_out, (int) (1 + y), 1, "%s", output_buffer[y]);
        wattroff(w_out, COLOR_PAIR(4));
      }
      memset(input_buffer, 0, LEN_INPUT_BUFFER);
      cursor_p = 0;
      clear_input(w_in);
      wrefresh(w_in);
      wrefresh(w_out);
    } else if (cursor_p < LEN_INPUT_BUFFER && cursor_p >= 0) {
      if (ch == KEY_BACKSPACE) {
        cursor_p = cursor_p == 0 ? cursor_p : cursor_p - 1;
        input_buffer[cursor_p] = '\0';
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
#else
#define LEX_IMPL
#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>

void handle_exit(int exit_code, void *args) {
  (void) args;
  log_eval_crash_type((eval_crash_t) exit_code);
}

int main(void) {
  on_exit(handle_exit, NULL);
  char *expr = "(1 + 21 + 3) * (5 + 72)";
  TokenStream stream = lex_expr(expr);
  tkn_trace_stream(&stream);
}

#endif // PARSE_TEST

void clear_input(WINDOW *w_in) {
  wmove(w_in, 1, 3);
  wclrtoeol(w_in);
  box(w_in, 0, 0);
}

void clear_output(WINDOW *w_out) {
  wmove(w_out, 1, 1);
  wclrtobot(w_out);
  box(w_out, 0, 0);
}
