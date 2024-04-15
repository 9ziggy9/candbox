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
  token_kind kind;
  struct ASTBinaryNode *lhs, *rhs;
  double value;
} ASTBinaryNode;
ASTBinaryNode * // MUST be pointer due to tree structure
ast_node_init(token_kind, double, ASTBinaryNode *, ASTBinaryNode *);

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
ASTBinaryNode *
ast_node_init(token_kind k, double v, ASTBinaryNode *lhs, ASTBinaryNode *rhs)
{
  ASTBinaryNode *node = (ASTBinaryNode *) malloc(sizeof(ASTBinaryNode));
  if (node == NULL) exit(EXIT_PARSE_NODE_ALLOC_FAIL);
  node->kind = k; node->value = v; node->lhs = lhs; node->rhs = rhs;
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
ASTBinaryNode* parse_stream(TokenStream *ts) {
  OpStack op_stack = op_stack_init(MAX_PARSE);
  ASTBinaryNode *node_stack[MAX_PARSE];  // TODO: refactor
  int node_sp = 0;

  Token token;
  while ((token = next_token(ts)).kind != TERMINATOR) {
    switch (token.kind) {
    case VALUE:
      node_stack[node_sp++] = ast_node_init(VALUE, token.value, NULL, NULL);
      break;
    case OPAREN:
      op_push(&op_stack, token);
      break;
    case CPAREN:
      while (op_stack.sp > 0 && op_stack.ops[op_stack.sp - 1].kind != OPAREN)
      {
        ASTBinaryNode *rhs = node_stack[--node_sp];
        ASTBinaryNode *lhs = node_stack[--node_sp];
        Token op = op_pop(&op_stack);
        node_stack[node_sp++] = ast_node_init(op.kind, 0, lhs, rhs);
      }
      if (op_stack.sp == 0 || op_stack.ops[op_stack.sp - 1].kind != OPAREN)
        exit(EXIT_PARSE_UNBALANCED_PARENS);
      op_pop(&op_stack);
      break;
    default:  // op_stack
      while (op_stack.sp > 0 &&
             precedence(op_stack.ops[op_stack.sp - 1].kind) >=
             precedence(token.kind) &&
             !is_right_associative(token.kind)) {
        ASTBinaryNode *rhs = node_stack[--node_sp];
        ASTBinaryNode *lhs = node_stack[--node_sp];
        Token op = op_pop(&op_stack);
        node_stack[node_sp++] = ast_node_init(op.kind, 0, lhs, rhs);
      }
      op_push(&op_stack, token);
    }
  }

  // Final reduction to a single AST
  while (op_stack.sp > 0) {
    ASTBinaryNode *rhs = node_stack[--node_sp];
    ASTBinaryNode *lhs = node_stack[--node_sp];
    Token op = op_pop(&op_stack);
    node_stack[node_sp++] = ast_node_init(op.kind, 0, lhs, rhs);
  }

  if (node_sp != 1) exit(EXIT_PARSE_UNBALANCED_PARENS);
  return node_stack[0];  // The root of the AST
}

void ast_parse_trace(ASTBinaryNode *node, int depth) {
  if (node == NULL) return;
  // print spaces for current depth level
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }

  switch (node->kind) {
  case VALUE: printf("%.2f\n", node->value); break;
  case OP_ADD: printf("+\n"); break;
  case OP_SUB: printf("-\n"); break;
  case OP_MUL: printf("*\n"); break;
  case OP_DIV: printf("/\n"); break;
  default: printf("Unknown\n");  // UNREACHABLE
  }
  ast_parse_trace(node->lhs, depth + 1);
  ast_parse_trace(node->rhs, depth + 1);
}

#endif // PARSE_IMPL
#endif // PARSE_H_
