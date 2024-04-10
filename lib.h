#ifndef LIB_H_
#define LIB_H_
#include <stdio.h>

void hello(void);

#ifdef LIB_IMPL
void hello(void) {
  printf("Hello, world!\n");
}
#endif

#endif // LIB_H_
