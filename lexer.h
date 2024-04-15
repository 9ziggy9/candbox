#ifndef LEXER_H_
#define LEXER_H_

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum { // PEMDAS
  OP_SUB, OP_ADD, OP_MUL, OP_DIV,
  OPAREN, CPAREN, VALUE, TERMINATOR,
} token_kind;

typedef struct { token_kind kind; double value; } Token;

#define MAX_TOKEN_STREAM 256
typedef struct {
  Token *tks;
  size_t n_tks;
  size_t cap;
} TokenStream;
void token_append(TokenStream *, Token);
TokenStream lex_expr(char *);
void token_stream_trace(TokenStream *);

typedef enum {
  EXIT_STREAM_OVERFLOW = 9000,
  EXIT_STREAM_ALLOC_FAIL,
  EXIT_STREAM_INVALID_CHAR,
} error_stream_t;
void token_print_error(error_stream_t);

#ifdef LEXER_IMPL
#define LOG_BREAK_ERR(S) case S: fprintf(stderr, ""#S"\n"); break;
void token_print_error(error_stream_t e) {
  if (e >= EXIT_STREAM_OVERFLOW && e <= 9100) {
    fprintf(stderr, "%s() :: ", __func__);
    switch (e) {
    LOG_BREAK_ERR(EXIT_STREAM_OVERFLOW);
    LOG_BREAK_ERR(EXIT_STREAM_ALLOC_FAIL);
    LOG_BREAK_ERR(EXIT_STREAM_INVALID_CHAR);
    }
  }
}

void token_append(TokenStream *stream, Token tk) {
  if (stream->n_tks >= stream->cap) exit(EXIT_STREAM_OVERFLOW);
  stream->tks[stream->n_tks++] = tk;
}

Token next_token(TokenStream *ts) {
  if (ts->n_tks > 0) {
    Token tk = ts->tks[0];  // Get the first token in the array
    ts->tks++;             // Move the array pointer to the next token
    ts->n_tks--;           // Decrement the count of tokens
    return tk;
  }
  // Return a token that acts as the end-of-stream indicator
  Token end_token = { .kind = TERMINATOR };
  return end_token;
}

TokenStream lex_expr(char *expr) {
  TokenStream stream = { .n_tks = 0, .cap = MAX_TOKEN_STREAM, .tks = NULL };
  stream.tks = (Token *) malloc(stream.cap * sizeof(Token));
  if (stream.tks == NULL) exit(EXIT_STREAM_ALLOC_FAIL);
  while (*expr != '\0') {
    if (isspace(*expr)) expr++;
    switch (*expr) {
    case '+': token_append(&stream, (Token) { .kind = OP_ADD }); expr++; break;
    case '-': token_append(&stream, (Token) { .kind = OP_SUB }); expr++; break;
    case '*': token_append(&stream, (Token) { .kind = OP_MUL }); expr++; break;
    case '/': token_append(&stream, (Token) { .kind = OP_DIV }); expr++; break;
    case '(': token_append(&stream, (Token) { .kind = OPAREN }); expr++; break;
    case ')': token_append(&stream, (Token) { .kind = CPAREN }); expr++; break;
    default : {
      if (!isdigit(*expr)) {
        fprintf(stderr, "bad char: %c\n", *expr);
        exit(EXIT_STREAM_INVALID_CHAR); 
      }
      long x = strtol(expr, &expr, 10);
      token_append(&stream, (Token) { .kind = VALUE, .value = (double) x });
    }
    }
  }
  return stream;
}

#define STR_FROM_OP(OP) ""#OP""
void token_stream_trace(TokenStream *stream) {
  if (stream->n_tks == 0) printf("-- EMPTY STREAM --\n");
  size_t count = 0;
  while (count < stream->n_tks) {
    switch (stream->tks[count].kind) {
    case OP_SUB: printf("[op: %s]", STR_FROM_OP(OP_SUB)); break;
    case OP_MUL: printf("[op: %s]", STR_FROM_OP(OP_MUL)); break;
    case OP_ADD: printf("[op: %s]", STR_FROM_OP(OP_ADD)); break;
    case OP_DIV: printf("[op: %s]", STR_FROM_OP(OP_DIV)); break;
    case OPAREN: printf("[op: %s]", STR_FROM_OP(OPAREN)); break;
    case CPAREN: printf("[op: %s]", STR_FROM_OP(CPAREN)); break;
    case VALUE:
      printf("[%s: %lf]", STR_FROM_OP(VALUE), stream->tks[count].value);
      break;
    }
    count++;
    printf(count < stream->n_tks ? " -> " : "\n");
  }
}

#endif // LEXER_IMPL
#endif // LEXER_H_
