#ifndef PARSE_H_
#define PARSE_H_
#include <stdbool.h>
#include "lexer.h"

typedef enum {
  EXIT_PARSE_OVERFLOW = 8000   ,
  EXIT_PARSE_UNDERFLOW         ,
  EXIT_PARSE_INVALID_CHAR      ,
  EXIT_PARSE_NODE_ALLOC_FAIL   ,
  EXIT_PARSE_STACK_ALLOC_FAIL  ,
  EXIT_PARSE_UNBALANCED_PARENS ,
} error_parse_t;

typedef struct ASTBinaryNode {
  struct ASTBinaryNode *lhs, *rhs;
  token_kind kind;
  ValueNumeric value;
} ASTBinaryNode;
// ast_node_init(): MUST return pointer for tree manipulation
ASTBinaryNode *ast_node_init(Token, ASTBinaryNode *, ASTBinaryNode *);

typedef struct {
  Token *ops;
  size_t sp;
  size_t cap;
} OpStack;
OpStack op_stack_init(size_t cap);
void    op_push(OpStack *, Token);
Token   op_pop(OpStack *);

ASTBinaryNode *parse_stream(TokenStream *);
void ast_parse_trace(ASTBinaryNode *, int);

#ifdef PARSE_IMPL
ASTBinaryNode *ast_node_init(Token tk, ASTBinaryNode *lhs, ASTBinaryNode *rhs)
{
  ASTBinaryNode *node = (ASTBinaryNode *) malloc(sizeof(ASTBinaryNode));
  if (node == NULL) exit(EXIT_PARSE_NODE_ALLOC_FAIL);
  node->kind = tk.kind;
  node->value = tk.value;
  node->lhs = lhs; node->rhs = rhs;
  return node;
}

OpStack op_stack_init(size_t cap) {
  Token *ops = (Token *) malloc(cap * sizeof(Token));
  if (ops == NULL) exit(EXIT_PARSE_STACK_ALLOC_FAIL);
  return (OpStack) { .ops = ops, .sp  = 0, .cap = cap };
}

void op_push(OpStack *stack, Token op) {
  if (stack->sp >= stack->cap) exit(EXIT_PARSE_OVERFLOW);
  stack->ops[stack->sp++] = op;
}

Token op_pop(OpStack *stack) {
  if (stack->sp <= 0) exit(EXIT_PARSE_UNDERFLOW);
  return stack->ops[--stack->sp];
}

// Helper functions
static int precedence(token_kind op) {
  switch(op) {
  case OP_MUL: case OP_DIV: return 2;
  case OP_ADD: case OP_SUB: return 1;
  // no ops
  case CPAREN: case OPAREN:
  case NUMERIC_I: case NUMERIC_F:
  case TERMINATOR:
  default: return 0;
  }
}

static inline bool is_bin_op(Token tk) {
  return tk.kind == OP_MUL || tk.kind == OP_DIV ||
         tk.kind == OP_ADD || tk.kind == OP_SUB;
}

static int is_right_associative(token_kind op) {
  (void) op;
  return 0; // all ops are left associative for now
}

#define MAX_PARSE 256
ASTBinaryNode* parse_stream(TokenStream *ts) { // refactor me
  OpStack op_stack = op_stack_init(MAX_PARSE);
  ASTBinaryNode *node_stack[MAX_PARSE];
  size_t node_sp = 0;

  Token tkn;
  while ((tkn = next_token(ts)).kind != TERMINATOR) {
    switch (tkn.kind) {
    case NUMERIC_F:
    case NUMERIC_I:
        node_stack[node_sp++] = ast_node_init(tkn, NULL, NULL); break;
    case OPAREN: op_push(&op_stack, tkn); break;
    case CPAREN:
      while (op_stack.sp > 0 &&
             op_stack.ops[op_stack.sp - 1].kind != OPAREN)
      {
        ASTBinaryNode *rhs = node_stack[--node_sp];
        ASTBinaryNode *lhs = node_stack[--node_sp];
        Token op = op_pop(&op_stack);
        node_stack[node_sp++] = ast_node_init(op, lhs, rhs);
      }
      if (op_stack.sp == 0 || op_stack.ops[op_stack.sp - 1].kind != OPAREN)
        exit(EXIT_PARSE_UNBALANCED_PARENS);
      op_pop(&op_stack);
      break;
    case OP_SUB: case OP_MUL: case OP_ADD: case OP_DIV: case TERMINATOR:
    default:  // op_stack
      while (op_stack.sp > 0 &&
             precedence(op_stack.ops[op_stack.sp - 1].kind) >=
             precedence(tkn.kind) &&
             !is_right_associative(tkn.kind)) {
        ASTBinaryNode *rhs = node_stack[--node_sp];
        ASTBinaryNode *lhs = node_stack[--node_sp];
        Token op = op_pop(&op_stack);
        node_stack[node_sp++] = ast_node_init(op, lhs, rhs);
      }
      op_push(&op_stack, tkn);
    }
  }

  // final reduction to a single AST
  while (op_stack.sp > 0) {
    ASTBinaryNode *rhs = node_stack[--node_sp];
    ASTBinaryNode *lhs = node_stack[--node_sp];
    Token op = op_pop(&op_stack);
    node_stack[node_sp++] = ast_node_init(op, lhs, rhs);
  }

  if (node_sp != 1) exit(EXIT_PARSE_UNBALANCED_PARENS);
  return node_stack[0];
}

void ast_parse_trace(ASTBinaryNode *node, int depth) {
  if (node == NULL) return;
  // print spaces for current depth level
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }

  switch (node->kind) {
  case NUMERIC_F: printf("%.2lf\n", node->value.fval); break;
  case NUMERIC_I: printf("%ld\n", node->value.ival); break;
  case OP_ADD: printf("+\n"); break;
  case OP_SUB: printf("-\n"); break;
  case OP_MUL: printf("*\n"); break;
  case OP_DIV: printf("/\n"); break;
  case OPAREN: case CPAREN: case TERMINATOR:
  default: printf("Unknown\n");  // UNREACHABLE
  }
  ast_parse_trace(node->lhs, depth + 1);
  ast_parse_trace(node->rhs, depth + 1);
}

#endif // PARSE_IMPL
#endif // PARSE_H_
