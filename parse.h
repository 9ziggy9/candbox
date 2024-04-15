#ifndef PARSE_H_
#define PARSE_H_
#include <stdio.h>
#include <stdlib.h>

typedef enum {
  EXIT_EVAL_STACK_OVERFLOW = 1000,
  EXIT_EVAL_STACK_UNDERFLOW,
  EXIT_EVAL_DIV_ZERO,
  EXIT_EVAL_BAD_EXPR,
  EXIT_EVAL_UNBALANCED,
  EXIT_EVAL_ALLOC_FAIL,
} eval_crash_t;
void log_eval_crash_type(eval_crash_t);
#define LOG_CASE_BRK(T) case T: fprintf(stderr, ""#T""); break;

void log_eval_crash_type(eval_crash_t exit_code) {
  if (exit_code >= EXIT_EVAL_STACK_OVERFLOW &&
      exit_code <= EXIT_EVAL_ALLOC_FAIL)
  {
    fprintf(stderr, "FATAL :: %s() :: ", __func__);
    switch(exit_code) {
      LOG_CASE_BRK(EXIT_EVAL_STACK_OVERFLOW);
      LOG_CASE_BRK(EXIT_EVAL_STACK_UNDERFLOW);
      LOG_CASE_BRK(EXIT_EVAL_DIV_ZERO);
      LOG_CASE_BRK(EXIT_EVAL_BAD_EXPR);
      LOG_CASE_BRK(EXIT_EVAL_UNBALANCED);
      LOG_CASE_BRK(EXIT_EVAL_ALLOC_FAIL);
    }
    fprintf(stderr, "\n");
  }
}

typedef enum {
  OP_SUB, OP_ADD, OP_DIV, OP_MUL,
  OP_OPAREN, OP_CPAREN, OPRND,
} token_kind;

typedef struct { token_kind kind; double data; } Token;

typedef struct {
  Token *tks;
  size_t sp;
  size_t cap;
} TokenStack;
void       tkn_push(TokenStack *, Token);
Token      tkn_pop(TokenStack *);
void       tkn_trace_stack(TokenStack *);
TokenStack parse_expr(char *);

#ifdef PARSE_IMPL
#include <ctype.h>
#include <math.h>
#include <stdio.h>

void tkn_push(TokenStack *stack, Token t) {
  if (stack->sp >= stack->cap) exit(EXIT_EVAL_STACK_OVERFLOW);
  stack->tks[++stack->sp] = t;
}
Token tkn_pop(TokenStack *stack) {
  if (stack->sp <= 0) exit(EXIT_EVAL_STACK_UNDERFLOW);
  return stack->tks[stack->sp--];
}

#define TKN_STACK_CAP 256
TokenStack parse_expr(char *expr) {
// BEGIN: alloc tkn stack
  TokenStack stack = { .tks = NULL, .sp = 0, .cap = TKN_STACK_CAP };
  stack.tks = (Token *) malloc(stack.cap * sizeof(Token));
  if (stack.tks == NULL) exit(EXIT_EVAL_ALLOC_FAIL);
// END: alloc tkn stack
  while (*expr != '\0') {
    while (isspace(*expr)) expr++;
    switch (*expr) {
    case '+': tkn_push(&stack, (Token) { .kind = OP_ADD });    expr++; break;
    case '-': tkn_push(&stack, (Token) { .kind = OP_SUB });    expr++; break;
    case '*': tkn_push(&stack, (Token) { .kind = OP_MUL });    expr++; break;
    case '/': tkn_push(&stack, (Token) { .kind = OP_DIV });    expr++; break;
    case '(': tkn_push(&stack, (Token) { .kind = OP_OPAREN }); expr++; break;
    case ')': tkn_push(&stack, (Token) { .kind = OP_CPAREN }); expr++; break;
    default: {
      if (!isdigit(*expr)) exit(EXIT_EVAL_BAD_EXPR);
      long x = strtol(expr, &expr, 10);
      tkn_push(&stack, (Token) { .kind = OPRND, .data = (double) x });
    }
    }
  }
  return stack;
}

#define STR_FROM_OP(O) ""#O""
void tkn_trace_stack(TokenStack *stack) {
  size_t sp = stack->sp;
  if (sp <= 0) printf("-- EMPTY STACK --\n");
  while (sp > 0) {
    switch (stack->tks[sp].kind) {
    case OP_SUB:    printf("op    -> %s\n", STR_FROM_OP(OP_SUB));    break;
    case OP_ADD:    printf("op    -> %s\n", STR_FROM_OP(OP_ADD));    break;
    case OP_MUL:    printf("op    -> %s\n", STR_FROM_OP(OP_MUL));    break;
    case OP_DIV:    printf("op    -> %s\n", STR_FROM_OP(OP_DIV));    break;
    case OP_OPAREN: printf("op    -> %s\n", STR_FROM_OP(OP_OPAREN)); break;
    case OP_CPAREN: printf("op    -> %s\n", STR_FROM_OP(OP_CPAREN)); break;
    case OPRND: default:
      printf("value -> %lf\n", stack->tks[sp].data);
    }
    sp--;
  }
}

#endif // PARSE_IMPL

#endif // PARSE_H_
