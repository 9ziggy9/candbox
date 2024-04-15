#ifndef LEX_H_
#define LEX_H_
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

typedef enum {
  OP_SUB, OP_ADD, OP_DIV, OP_MUL,
  OP_OPAREN, OP_CPAREN, OPRND,
} token_kind;

typedef struct { token_kind kind; double data; } Token;

typedef struct {
  Token *tks;
  size_t n_tkns;
  size_t cap;
} TokenStream;
void       tkn_append(TokenStream *, Token);
void       tkn_trace_stream(TokenStream *);
TokenStream lex_expr(char *);

#ifdef LEX_IMPL
#include <ctype.h>
#include <math.h>
#include <stdio.h>

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

void tkn_append(TokenStream *stream, Token t) {
  if (stream->n_tkns >= stream->cap) exit(EXIT_EVAL_STACK_OVERFLOW);
  stream->tks[++stream->n_tkns] = t;
}

#define TKN_STREAM_CAP 256
TokenStream lex_expr(char *expr) {
// BEGIN: alloc tkn stream
  TokenStream stream = { .tks = NULL, .n_tkns = 0, .cap = TKN_STREAM_CAP };
  stream.tks = (Token *) malloc(stream.cap * sizeof(Token));
  if (stream.tks == NULL) exit(EXIT_EVAL_ALLOC_FAIL);
// END: alloc tkn stream
  while (*expr != '\0') {
    while (isspace(*expr)) expr++;
    switch (*expr) {
    case '+': tkn_append(&stream, (Token) { .kind = OP_ADD });    expr++; break;
    case '-': tkn_append(&stream, (Token) { .kind = OP_SUB });    expr++; break;
    case '*': tkn_append(&stream, (Token) { .kind = OP_MUL });    expr++; break;
    case '/': tkn_append(&stream, (Token) { .kind = OP_DIV });    expr++; break;
    case '(': tkn_append(&stream, (Token) { .kind = OP_OPAREN }); expr++; break;
    case ')': tkn_append(&stream, (Token) { .kind = OP_CPAREN }); expr++; break;
    default: {
      if (!isdigit(*expr)) exit(EXIT_EVAL_BAD_EXPR);
      long x = strtol(expr, &expr, 10);
      tkn_append(&stream, (Token) { .kind = OPRND, .data = (double) x });
    }
    }
  }
  return stream;
}

#define STR_FROM_OP(O) ""#O""
void tkn_trace_stream(TokenStream *stream) {
  size_t n_tkns = stream->n_tkns;
  if (n_tkns <= 0) printf("-- EMPTY STREAM --\n");
  while (n_tkns > 0) {
    switch (stream->tks[n_tkns].kind) {
    case OP_SUB:    printf("(op: %s) -> ", STR_FROM_OP(OP_SUB));    break;
    case OP_ADD:    printf("(op: %s) -> ", STR_FROM_OP(OP_ADD));    break;
    case OP_MUL:    printf("(op: %s) -> ", STR_FROM_OP(OP_MUL));    break;
    case OP_DIV:    printf("(op: %s) -> ", STR_FROM_OP(OP_DIV));    break;
    case OP_OPAREN: printf("(op: %s) -> ", STR_FROM_OP(OP_OPAREN)); break;
    case OP_CPAREN: printf("(op: %s) -> ", STR_FROM_OP(OP_CPAREN)); break;
    case OPRND: default:
      printf("(value: %lf) -> ", stream->tks[n_tkns].data);
    }
    n_tkns--;
  }
  printf("\n");
}

#endif // LEX_IMPL
#endif // LEX_H_
