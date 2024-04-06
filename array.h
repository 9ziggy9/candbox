#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

typedef enum { FIXED, DYNAMIC } ArrVariant;

#define __ARR_INIT_CAP__  32
#define __ARR_GROW_RATE__ 2
#define __ARR_RESIZE_CAP__(C) C == 0                                          \
    ? __ARR_INIT_CAP__                                                        \
    : __ARR_GROW_RATE__ * C                                                   \

#define ARRAY(T) struct {                                                     \
  T *  elems;                                                                 \
  size_t cap;                                                                 \
  size_t len;                                                                 \
  ArrVariant variant;                                                         \
}                                                                             \

#define ARRAY_PUSH(XS, X)                                                     \
  do {                                                                        \
      if((XS)->variant == FIXED) {                                            \
        TRACE_POSITION                                                        \
        fprintf(stderr, "cannot push into fixed array\n");                    \
        exit(EXIT_FAILURE);                                                   \
      }                                                                       \
      if ((XS)->len >= (XS)->cap) {                                           \
        (XS)->cap = __ARR_RESIZE_CAP__((XS)->cap);                            \
        (XS)->elems = realloc((XS)->elems, (XS)->cap * sizeof(*(XS)->elems)); \
      }                                                                       \
      (XS)->elems[(XS)->len++] = (X);                                         \
  } while(0);                                                                 \

#define ARRAY_CAT(XS, YS)                                                     \
  do {                                                                        \
    if((XS)->variant != (YS)->variant) {                                      \
      TRACE_POSITION                                                          \
      fprintf(stderr, "cannot concatenate mixed variants\n");                 \
      exit(EXIT_FAILURE);                                                     \
    }                                                                         \
    if (((XS)->len + (YS)->len) > (XS)->cap) {                                \
      if ((XS)->cap == 0) (XS)->cap = __ARR_INIT_CAP__;                       \
      while (((XS)->len + (YS)->len) > (XS)->cap) (XS)->cap *= 2;             \
      (XS)->elems = realloc((XS)->elems, (XS)->cap * sizeof(*(XS)->elems));   \
    }                                                                         \
    memcpy(((XS)->elems + (XS)->len),                                         \
            (YS)->elems, (YS)->len * sizeof(*(XS)->elems));                   \
    (XS)->len += (YS)->len;                                                   \
  } while(0);                                                                 \

#define FOR_EACH(X, XS, FN)                                                   \

#define ARRAY_INIT(V, C)                                                      \
  { .elems = NULL, .cap = C, .len = V == FIXED ? C : 0, .variant = V }        \

#endif // ARRAY_H_

/*

  TODO: ARRAY_FROM()
  TODO: FOR_EACH(), MAP(), FILTER()
  TODO: error checking in general

*/
