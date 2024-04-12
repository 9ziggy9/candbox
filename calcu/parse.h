#ifndef PARSE_H_
#define PARSE_H_
#include <stdlib.h>
#include <stdint.h>

typedef enum {
  OP_SUB, OP_ADD, OP_DIV,
  OP_MUL, OP_PAREN, VALUE_NUM,
} token_kind;

typedef struct {
  token_kind kind;
  double value;
} Token;

typedef struct {
  Token *tks;
  size_t sp;
  size_t cap;
} RPNStack;

void  rpn_push(RPNStack *, Token);
Token rpn_pop(RPNStack *);
Token rpn_peek(const RPNStack *);
void  rpn_trace_stack(RPNStack *);
void  rpn(RPNStack *);

RPNStack parse_expr(char *);

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

void rpn_push(RPNStack *stack, Token tok) {
  guard_stack_overflow(stack->sp, stack->cap);
  stack->tks[++stack->sp] = tok;
}

Token rpn_pop(RPNStack *stack) {
  guard_stack_underflow(stack->sp);
  return stack->tks[stack->sp--];
}

Token rpn_peek(const RPNStack *stack) {
  if (stack->sp == 0) {
    Token empty_token = { .kind = VALUE_NUM, .value = 0 };
    return empty_token;
  }
  return stack->tks[stack->sp - 1];
}

#define RPN_STACK_CAP 256
RPNStack parse_expr(char *expr) {
// BEGIN: alloc rpn stack
  RPNStack stack = { .tks = NULL, .sp = 0, .cap = RPN_STACK_CAP };
  stack.tks = (Token *) malloc(stack.cap * sizeof(Token));
  if (stack.tks == NULL) exit(EXIT_EVAL_ALLOC_FAIL); 
// END: alloc rpn stack
// BEGIN: parse expr
  while (*expr != '\0') {
    while(isspace(*expr)) expr++;
    switch (*expr) {
    case '+': rpn_push(&stack, (Token) { .kind = OP_ADD }); expr++; break;
    case '-': rpn_push(&stack, (Token) { .kind = OP_SUB }); expr++; break;
    case '*': rpn_push(&stack, (Token) { .kind = OP_MUL }); expr++; break;
    case '/': rpn_push(&stack, (Token) { .kind = OP_DIV }); expr++; break;
    default: {
      if (!isdigit(*expr)) exit(EXIT_EVAL_BAD_EXPR);
      long x = strtol(expr, &expr, 10);
      rpn_push(&stack, (Token) { .kind = VALUE_NUM, .value = (double) x});
    }
    }
  }
// END: parse expr
  return stack;
}

#define STR_FROM_OP(O) ""#O""
void rpn_trace_stack(RPNStack *stack) {
  size_t sp = stack->sp;
  if (sp <= 0) printf("-- EMPTY STACK --\n");
  while (sp > 0) {
    switch (stack->tks[sp].kind) {
    case OP_SUB: printf("op    -> %s\n", STR_FROM_OP(OP_SUB)); break;
    case OP_ADD: printf("op    -> %s\n", STR_FROM_OP(OP_ADD)); break;
    case OP_MUL: printf("op    -> %s\n", STR_FROM_OP(OP_MUL)); break;
    case OP_DIV: printf("op    -> %s\n", STR_FROM_OP(OP_DIV)); break;
    default: printf("value -> %lf\n", stack->tks[sp].value);
    }
    sp--;
  }
}

double eval_rpn(RPNStack *stack, token_t op, double acc) {
  if (stack->sp == 0) return acc;

  Token top = rpn_pop(stack);
  if (top.kind == VALUE_NUM) eval_rpn(stack, op, acc + apply_op(top.value));
  else eval_rpn(stack, top, acc);
}

#endif // PARSE_IMPL
#endif //PARSE_H_
