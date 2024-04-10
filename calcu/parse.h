#ifndef PARSE_H_
#define PARSE_H_
#include <stdlib.h>

typedef struct {
  double *xs; // data points
  size_t sp;  // index of top data (think stack pointer)
  size_t cap; // (sp > cap) => stack overflow
} EvalStack;
void push(EvalStack *, double);
double pop(EvalStack *);
double eval_expr(EvalStack *, char *);

typedef enum {
  EXIT_EVAL_STACK_OVERFLOW = 1000,
  EXIT_EVAL_STACK_UNDERFLOW,
  EXIT_EVAL_DIV_ZERO,
  EXIT_EVAL_BAD_EXPR,
  EXIT_EVAL_UNBALANCED,
} eval_crash_t;
void log_eval_crash_type(eval_crash_t);
#define LOG_CASE_BRK(T) case T: fprintf(stderr, ""#T""); break;

#ifdef PARSE_IMPL
#include <ctype.h>
#include <math.h>
#include <stdio.h>

static void guard_stack_overflow(size_t sp, size_t cap) {
  if (sp >= cap) exit(EXIT_EVAL_STACK_OVERFLOW);
}

static void guard_stack_underflow(size_t sp) {
  if (sp == 0) exit(EXIT_EVAL_STACK_UNDERFLOW);
}

void push(EvalStack *stack, double x) {
  guard_stack_overflow(stack->sp, stack->cap);
  stack->xs[++stack->sp] = x;
}

void log_eval_crash_type(eval_crash_t exit_code) {
  if (exit_code >= EXIT_EVAL_STACK_OVERFLOW &&
      exit_code <= EXIT_EVAL_UNBALANCED)
  {
    fprintf(stderr, "FATAL :: %s() :: ", __func__);
    switch(exit_code) {
    LOG_CASE_BRK(EXIT_EVAL_STACK_OVERFLOW);
    LOG_CASE_BRK(EXIT_EVAL_STACK_UNDERFLOW);
    LOG_CASE_BRK(EXIT_EVAL_DIV_ZERO);
    LOG_CASE_BRK(EXIT_EVAL_BAD_EXPR);
    LOG_CASE_BRK(EXIT_EVAL_UNBALANCED);
    }
    fprintf(stderr, "\n");
  }
}

double pop(EvalStack *stack) {
  guard_stack_underflow(stack->sp);
  return stack->xs[stack->sp--];
}

double eval_expr(EvalStack *stack, char *expr) {
  while(*expr != '\0') {
    while(isspace(*expr)) expr++;
    switch(*expr) {
    case '+': push(stack, pop(stack) + pop(stack)); expr++; break;
    case '-': {
      double x = pop(stack);
      double y = pop(stack);
      push(stack, y - x);
      expr++;
      break;
    }
    case '*': push(stack, pop(stack) * pop(stack)); expr++; break;
    case '/': {
      double x = pop(stack);
      double y = pop(stack);
      if (x == 0) exit(EXIT_EVAL_DIV_ZERO);
      push(stack, y / x); expr++; break;
    }
    default: {
      if (!isdigit(*expr)) exit(EXIT_EVAL_BAD_EXPR);
      long x = strtol(expr, &expr, 10);
      push(stack, (double) x);
    }
    }
  }
  if (stack->sp != 1) exit(EXIT_EVAL_UNBALANCED);
  return pop(stack);
}

#endif // PARSE_IMPL
#endif //PARSE_H_
