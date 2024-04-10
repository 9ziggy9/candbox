#define PARSE_IMPL
#include "parse.h"

int main(void) {
  EvalStack stack;
  stack.cap = 100;
  stack.sp = 0;
  stack.xs = malloc(stack.cap * sizeof(double));

  char *expr = "24 45 + 42 1000 * +";
  printf("Result: %.2lf\n", eval_expr(&stack, expr));

  free(stack.xs);
  return 0;
}
