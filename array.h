#ifndef ARRAY_H_
#define ARRAY_H_
#include "log.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum { SUCCESS, FAILURE } ArrayOpResult;
typedef enum { FIXED, DYNAMIC } ArrayVariant;

typedef struct {
  int *  elems;
  size_t cap;
  size_t length;
  ArrayVariant variant;
} Array;

#define __ARRAY_EXTRACT_SIZE__(XS) XS, sizeof(XS) / sizeof(XS[0])
#define __ARRAY_STACK_INJECT__(SZ) (int [SZ]){0}, SZ

#define ARRAY_INIT(V, XS) array_init(V, __ARRAY_EXTRACT_SIZE__(XS));
Array array_init(ArrayVariant, int *, size_t);

#define ARRAY_NEW(V, SZ) array_new(V, __ARRAY_STACK_INJECT__(SZ));
Array array_new(ArrayVariant, int *, size_t);

void array_log(const Array *);
ArrayOpResult array_push(Array *, int);
ArrayOpResult array_fix_push(Array *, int);
ArrayOpResult array_dyn_push(Array *, int);

Array array_dyn_init(ArrayVariant, int *, size_t);
Array array_fix_init(ArrayVariant, int *, size_t);
Array array_dyn_new (ArrayVariant, size_t);
Array array_fix_new (ArrayVariant, int *, size_t);

#ifdef ARRAY_IMPL
void array_print(const Array *xs) {
  TRACE_FUNCTION_NAME;
  printf("{ ");
  if (xs->length == 0) printf("EMPTY");
  else
    for (size_t n = 0; n < xs->length; n++) {
      printf("%d%s", xs->elems[n], (n < xs->length - 1) ? ", " : "\0");
    }
  printf(" }\n");
}

Array array_init(ArrayVariant v, int *xs, size_t sz) {
  return v == DYNAMIC
    ? array_dyn_init(v, xs, sz)
    : array_fix_init(v, xs, sz);
}

Array array_new(ArrayVariant v, int *xs, size_t sz) {
  return v == DYNAMIC
    ? array_dyn_new(v, sz)
    : array_fix_new(v, xs, sz);
}

Array array_dyn_init(ArrayVariant v, int *xs, size_t sz) {
  int *ys =  (int *) malloc(sizeof(int) * sz);
  for (size_t n = 0; n < sz; n++) ys[n] = xs[n];
  return (Array)
    { .cap = sz, .length = sz, .elems = ys, .variant = v };
}

Array array_dyn_new(ArrayVariant v, size_t sz) {
  int *xs =  (int *) calloc(sizeof(int) * sz, 0);
  return (Array)
    { .cap = sz, .length = 0, .elems = xs, .variant = v};
}

Array array_fix_init(ArrayVariant v, int *xs, size_t sz) {
  return (Array)
    { .cap = sz, .length = sz, .elems = xs, .variant = v };
}

Array array_fix_new(ArrayVariant v, int *xs, size_t sz) {
  return (Array)
    { .cap = sz, .length = 0, .elems = xs, .variant = v };
}

ArrayOpResult array_push(Array *xs, int x) {
  return xs->variant == DYNAMIC
    ? array_dyn_push(xs, x)
    : array_fix_push(xs, x);
}

ArrayOpResult array_dyn_push(Array *xs, int x) {
  if (xs->variant != DYNAMIC) return FAILURE;
  if (xs->length == xs->cap) {
    xs->cap *= 2;
    xs->elems = (int *)
      realloc(xs->elems, (sizeof(int)) * xs->cap);
  }
  xs->elems[xs->length++] = x;
  return SUCCESS;
}

ArrayOpResult array_fix_push(Array *xs, int x) {
  if (xs->length >= xs->cap || xs->variant != FIXED)
    return FAILURE;
  xs->elems[xs->length++] = x;
  return SUCCESS;
}

#endif // ARRAY_IMPL
#endif // ARRAY_H_
