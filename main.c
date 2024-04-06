#include "array.h"

int main(void) {
  ARRAY(char *) xs = ARRAY_INIT(DYNAMIC, 0);
  ARRAY(char *) ys = ARRAY_INIT(DYNAMIC, 0);

  ARRAY_PUSH(&xs, "hello");
  ARRAY_PUSH(&xs, "world");
  ARRAY_PUSH(&ys, "goodbye");
  ARRAY_PUSH(&ys, "moon");

  ARRAY_CAT(&xs, &ys);

  for (size_t i = 0; i < xs.len; i++) printf("%s\n", xs.elems[i]);
  return 0;
}
