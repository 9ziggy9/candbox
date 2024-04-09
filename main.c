#define LIST_IMPL
#include "list.h"

int main(void) {
  List lst = LIST_CREATE();
  LIST_APPEND(&lst, NODE_CREATE(int, 1));
  LIST_APPEND(&lst, NODE_CREATE(int, 2));
  LIST_APPEND(&lst, NODE_CREATE(int, 3));
  return 0;
}
