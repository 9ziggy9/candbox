#ifndef EVAL_H_
#define EVAL_H_

#include "lexer.h"
#include "parse.h"

double eval_ast(ASTBinaryNode *);

#ifdef EVAL_IMPL
double eval_ast(ASTBinaryNode *node) {
  if (node->kind == VALUE) {
    return node->value;
  } else {
    double lhs = eval_ast(node->lhs);
    double rhs = eval_ast(node->rhs);
    switch (node->kind) {
    case OP_ADD: return lhs + rhs;
    case OP_SUB: return lhs - rhs;
    case OP_MUL: return lhs * rhs;
    case OP_DIV: return lhs / rhs;  // beware of division by zero!
    case OPAREN: case CPAREN: case VALUE: case TERMINATOR:
    default: exit(EXIT_FAILURE); // implement me
    }
  }
}
#endif // EVAL_IMPL
#endif // EVAL_H_
