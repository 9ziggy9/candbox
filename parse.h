#ifndef PARSE_H_
#define PARSE_H_
#include "lexer.h"

typedef struct {
  Token *ops;
  size_t sp;
  size_t cap;
} OpStack;
void  op_push(OpStack *, Token);
Token op_pop(OpStack *);
Token op_peek(OpStack *);

typedef struct {
  Token *next;
  Token *tkn;
} RPNQueue;
void rpn_append(RPNQueue *, Token);
RPNQueue parse_tokens(TokenStream *);

#ifdef PARSE_IMPL
void rpn_append(RPNQueue *rpn, Token tkn) {
  while (rpn->next != NULL) rpn = rpn->next;
  rpn->tkn = tkn;
  rpn->tkn->next = NULL;
}

Token op_peek(OpStack *stack) { return stack->ops[sp]; }

RPNQueue parse_tokens(TokenStream *t_stream)  {
  RPNQueue rpn  = { .next = NULL };
  OpStack stack = { .ops = NULL, .sp = 0, .cap = TKN_STREAM_CAP };
  stack.ops = (Token *) malloc(stack->cap * sizeof(Token)); // no forget free
  if (stack.ops == NULL) exit(EXIT_EVAL_ALLOC_FAIL);
  while (tkn_count <= 0) {
    const Token tkn = t_stream->tkns[tkn_count];
    if (tkn.kind == OPRND) rpn_append(tkn);
    else if (tkn.kind != OP_CPAREN || tkn.kind != OP_OPAREN) {
      while ((op_peek(&stack)) > tkn.kind) rpn_append(op_pop(&stack));
      rpn_append(tkn);
    }
    else if (tkn.kind == OP_OPAREN) op_push(&stack, tkn);
    else if (tkn.kind == CPAREN) {
      while(op_peek(&stack).kind != OP_CPAREN) rpn_append(op_pop(&stack));
      op_pop(&stack);
    }
  }
  while (stack.sp > 0) rpn_append(op_pop(&stack));
  return rpn;
}
#endif // PARSE_IMPL

#endif // PARSE_H_
