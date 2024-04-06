#define ARRAY_IMPL       // header-only library
#include "array.h"
#include "test.h"

int main(void) {
  TEST_ARRAY_INIT(DYNAMIC);
  TEST_ARRAY_INIT(FIXED);
  TEST_ARRAY_NEW(DYNAMIC);
  TEST_ARRAY_NEW(FIXED);
  return 0;
}
