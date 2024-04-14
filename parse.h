#ifndef PARSE_H_
#define PARSE_H_
#include <stdio.h>
void hello(void);

#ifdef PARSE_IMPL
void hello(void) {
  printf("Hello, from parse!\n");
}
#endif //PARSE_IMPL
#endif // PARSE_H_
