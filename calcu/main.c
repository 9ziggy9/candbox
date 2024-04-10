#define PARSE_IMPL
#include "parse.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ui_init(void);
static void panic_null_win(WINDOW *);
static void refresh_screen(WINDOW *, WINDOW *);
static void clear_input(WINDOW *);
static void clear_output(WINDOW *);

#define INPUT_ORIGIN 1, 1
#define LEN_INPUT_BUFFER 256

void exit_clean(int exit_code, void *args) {
  WINDOW **ws = (WINDOW **) args;
  delwin(ws[0]);
  delwin(ws[1]);
  endwin();
  log_crash_type((eval_crash_t) exit_code);
}

int main(void) {
  ui_init();

  int height_screen = getmaxy(stdscr);
  int width_screen  = getmaxx(stdscr);

  WINDOW *w_out = newwin(height_screen - 5, width_screen - 2, 1, 1);
  panic_null_win(w_out);
  scrollok(w_out, TRUE);
  box(w_out, 0, 0);

  WINDOW *w_in = newwin(3, width_screen - 2, height_screen - 4, 1);
  panic_null_win(w_in);
  box(w_in, 0, 0);
  mvwprintw(w_in, INPUT_ORIGIN, "> ");

  // register windows to be cleaned up on abrupt exits
  on_exit(exit_clean, (WINDOW *[]){w_out, w_in});

  refresh_screen(w_out, w_in);

  char input_buffer[LEN_INPUT_BUFFER] = {0};
  size_t input_pos = 0;
  int ch; // take more than ASCII
  while((ch = wgetch(w_in)) != 27) {
    if (ch == '\n') {
      clear_output(w_out);
      mvwprintw(w_out, 1, 1, ">> %s", input_buffer);

      EvalStack ev_stack;
      ev_stack.cap = 100;
      ev_stack.sp = 0;
      ev_stack.xs = malloc(ev_stack.cap * sizeof(double));
      mvwprintw(w_out, 2, 1, "> %.2lf", eval_expr(&ev_stack, input_buffer));

      memset(input_buffer, 0, LEN_INPUT_BUFFER);
      input_pos = 0;
      wrefresh(w_out);
      clear_input(w_in);
    } else if (input_pos < LEN_INPUT_BUFFER - 1) {
      input_buffer[input_pos++] = (char) ch;
    }
    mvwprintw(w_in, 1, 3, "%s", input_buffer);
    wrefresh(w_in);
  }

  delwin(w_out);
  delwin(w_in);
  endwin();
  return 0;
}

static void panic_null_win(WINDOW *w) {
  if (w == NULL) {
    fprintf(stderr, "couldn't alloc win resources.\n");
    exit(EXIT_FAILURE);
  }
}

static void refresh_screen(WINDOW *w_out, WINDOW *w_in) {
  refresh();
  clear_output(w_out);
  clear_input(w_in);
}

static void clear_output(WINDOW *w) {
  werase(w);    // Clear input window
  box(w, 0, 0); // Redraw border for input window
  wrefresh(w);
}

static void clear_input(WINDOW *w) {
  wmove(w, 1, 3);
  wclrtoeol(w);
  wrefresh(w);
}

void ui_init(void) {
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  set_escdelay(0); // immediate escape key response
  curs_set(1);
}
