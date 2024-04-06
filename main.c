#include "array.h"

int main(void) {
  ARRAY(int) xs = ARRAY_INIT(DYNAMIC, 0);
  ARRAY(int) ys = ARRAY_INIT(DYNAMIC, 0);

  for (int x = 0; x < 10; ++x) ARRAY_PUSH(&xs, x);
  for (int y = 10; y > 0; y--) ARRAY_PUSH(&ys, y);

  ARRAY_CAT(&xs, &ys);

  for (size_t i = 0; i < xs.len; i++) printf("%d\n", xs.elems[i]);

  return 0;
}
