#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void ui_init(void);
static void panic_null_win(WINDOW *);
static void render_screen(WINDOW *, WINDOW *);
static void clear_input(WINDOW *);
static void clear_output(WINDOW *);
static char *sanitize_input(char *);

typedef struct {
  int   *xs;  // data points
  size_t sp;  // index of top data (think stack pointer)
  size_t cap; // (sp > cap) => stack overflow
} EvalStack;
void push(EvalStack *, int);
int pop(EvalStack *);
int eval_expr(EvalStack *, char *);

#define INPUT_ORIGIN 1, 1
#define LEN_INPUT_BUFFER 256

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

  render_screen(w_out, w_in);

  char input_buffer[LEN_INPUT_BUFFER] = {0};
  size_t input_pos = 0;
  int ch; // take more than ASCII
  while((ch = wgetch(w_in)) != 27) {
    if (ch == '\n') {
      clear_output(w_out);
      mvwprintw(w_out, 1, 1, ">> %s", input_buffer);
      EvalStack stack;
      stack.cap = 256;
      stack.sp = 0;
      stack.xs = (int *) malloc(sizeof(int) * stack.cap);
      mvwprintw(w_out, 2, 1, "> %d",
                eval_expr(&stack, sanitize_input(input_buffer)));
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

static char *sanitize_input(char *in) {
  char *out = (char *) malloc(strlen(in) + 1);
  char *pout = out;
  for (char *c = in; *c != '\0'; c++) {
    if (isdigit(*c) || strchr("+-*/", *c)) *pout++ = *c;
  }
  *pout = '\0';
  return out;
}

static void guard_stack_overflow(size_t sz, size_t cap) {
  if (sz >= cap) {
    fprintf(stderr, "STACK OVERFLOW!!!!\n");
    exit(EXIT_FAILURE);
  }
}

static void guard_stack_underflow(size_t sz) {
  if (sz == 0) {
    fprintf(stderr, "STACK UNDERFLOW!!!!\n");
    exit(EXIT_FAILURE);
  }
}

void push(EvalStack *s, int x) {
  guard_stack_overflow(s->sp, s->cap);
  s->xs[s->sp++] = x;
}

int pop(EvalStack *s) {
  guard_stack_underflow(s->sp);
  return s->xs[s->sp++];
}

int eval_expr(EvalStack *s, char *expr) {
  while(*expr != '\0') {
    switch(*expr) {
    case '+': push(s, pop(s) + pop(s)); break;
    case '-': push(s, pop(s) - pop(s)); break;
    case '*': push(s, pop(s) * pop(s)); break;
    case '/': push(s, pop(s) / pop(s)); break; // danger :D
    default: {
      if (isdigit(*expr)) {
        int x = 0;
        size_t digit = 0;
        while(isdigit(*expr)) x += pow(10, digit) * atoi(expr++);
        push(s, x);
      }
    }
    }
    expr++;
  }
  return s->xs[0];
}

static void panic_null_win(WINDOW *w) {
  if (w == NULL) {
    fprintf(stderr, "couldn't alloc win resources.\n");
    exit(EXIT_FAILURE);
  }
}

static void render_screen(WINDOW *w_out, WINDOW *w_in) {
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
  werase(w);    // Clear input window
  box(w, 0, 0); // Redraw border for input window
  mvwprintw(w, INPUT_ORIGIN, "> "); // Print prompt
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
