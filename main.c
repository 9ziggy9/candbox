#define ARRAY_IMPL      // header-only library
#include "array.h"

int main(void) {

  Array xs = ARRAY_INIT(((int []){1,2,3,4,5}))
  array_print(&xs);

  Array ys = ARRAY_NEW(10);
  array_print(&ys);

  return 0;
}
