#ifndef EVAL_H_
#define EVAL_H_

#include "lexer.h"
#include "parse.h"

ValueNumeric eval_ast(ASTBinaryNode *);

ValueNumeric eval_ast(ASTBinaryNode *node) {
  if (node->kind == NUMERIC_I || node->kind == NUMERIC_F) {
    return node->value;
  } else {
    ValueNumeric lhs = eval_ast(node->lhs);
    ValueNumeric rhs = eval_ast(node->rhs);
    ValueNumeric result;

    // Determine the type of the operation based on lhs and rhs
    bool is_float = (node->lhs->kind == NUMERIC_F ||
                     node->rhs->kind == NUMERIC_F);

    switch (node->kind) {
      case OP_ADD:
        if (is_float)
          result.fval = (lhs.kind == NUMERIC_F ? lhs.fval : lhs.ival) +
                        (rhs.kind == NUMERIC_F ? rhs.fval : rhs.ival);
        else
          result.ival = lhs.ival + rhs.ival;
        break;
      case OP_SUB:
        if (is_float)
          result.fval = (lhs.kind == NUMERIC_F ? lhs.fval : lhs.ival) -
                        (rhs.kind == NUMERIC_F ? rhs.fval : rhs.ival);
        else
          result.ival = lhs.ival - rhs.ival;
        break;
      case OP_MUL:
        if (is_float)
          result.fval = (lhs.kind == NUMERIC_F ? lhs.fval : lhs.ival) *
                        (rhs.kind == NUMERIC_F ? rhs.fval : rhs.ival);
        else
          result.ival = lhs.ival * rhs.ival;
        break;
      case OP_DIV:
        if (rhs.kind == NUMERIC_F && rhs.fval == 0) exit(EXIT_FAILURE);
        if (rhs.kind == NUMERIC_I && rhs.ival == 0) exit(EXIT_FAILURE);
        if (is_float)
          result.fval = (lhs.kind == NUMERIC_F ? lhs.fval : lhs.ival) /
                        (rhs.kind == NUMERIC_F ? rhs.fval : rhs.ival);
        else
          result.ival = lhs.ival / rhs.ival;
        break;
      default: exit(EXIT_FAILURE);
    }
    result.kind = is_float ? NUMERIC_F : NUMERIC_I;
    return result;
  }
}

#endif // EVAL_IMPL
#endif // EVAL_H_
