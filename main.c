/* #define ARRAY_IMPL */
/* #include "array.h" */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __ARR_INIT_CAP__  32
#define __ARR_GROW_RATE__ 2

#define __ARR_RESIZE_CAP__(C) C == 0 \
    ? __ARR_INIT_CAP__               \
    : __ARR_GROW_RATE__ * C          \

#define ARRAY(T) struct { T * elems; size_t cap; size_t len; }
#define ARRAY_PUSH(XS, X)                                                     \
  do {                                                                        \
      if ((XS)->len >= (XS)->cap) {                                           \
        (XS)->cap = __ARR_RESIZE_CAP__((XS)->cap);                            \
        (XS)->elems = realloc((XS)->elems, (XS)->cap * sizeof(*(XS)->elems)); \
      }                                                                       \
      (XS)->elems[(XS)->len++] = (X);                                         \
  } while(0);                                                                 \

#define ARRAY_CAT(XS, YS)                                                   \
  do {                                                                      \
    if (((XS)->len + (YS)->len) > (XS)->cap) {                              \
      if ((XS)->cap == 0) (XS)->cap = __ARR_INIT_CAP__;                     \
      while (((XS)->len + (YS)->len) > (XS)->cap) (XS)->cap *= 2;           \
      (XS)->elems = realloc((XS)->elems, (XS)->cap * sizeof(*(XS)->elems)); \
    }                                                                       \
    memcpy(((XS)->elems + (XS)->len),                                       \
            (YS)->elems, (YS)->len * sizeof(*(XS)->elems));                 \
    (XS)->len += (YS)->len;                                                 \
  } while(0);                                                               \

int main(void) {
  ARRAY(int) xs = {0};
  ARRAY(int) ys = {0};
  for (int x = 0; x < 10; ++x) ARRAY_PUSH(&xs, x);
  for (int y = 10; y > 0; y--) ARRAY_PUSH(&ys, y);
  ARRAY_CAT(&xs, &ys);
  for (size_t i = 0; i < xs.len; i++) printf("%d\n", xs.elems[i]);
  return 0;
}
