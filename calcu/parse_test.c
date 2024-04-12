#define PARSE_IMPL
#include "parse.h"
#include <stdlib.h>

void handle_exit(int exit_code, void *args) {
  (void) args;
  log_eval_crash_type((eval_crash_t) exit_code);
}

int main(void) {
  on_exit(handle_exit, NULL);

  char *expr = "2 24 * 24 -";
  RPNStack rpn_stack = parse_expr(expr);

  rpn_trace_stack(&rpn_stack);
  double x = eval_rpn(&rpn_stack, 0);
  printf("result: %lf\n", x);

  rpn_trace_stack(&rpn_stack);

  return 0;
}
