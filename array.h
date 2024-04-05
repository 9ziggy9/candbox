#ifndef ARRAY_H_
#define ARRAY_H_
#include "log.h"
#include <stdio.h>

typedef enum { SUCCESS = 0, FAILURE } ArrayOpResult;

typedef struct {
  int *elems;
  size_t cap;
  size_t length;
} Array;

#define __ARRAY_EXTRACT_SIZE__(xs) xs, sizeof(xs) / sizeof(xs[0])
#define __ARRAY_BIND_SIZE__(sz) ((int [sz]){0}), sz

#define ARRAY_INIT(xs) array_init(__ARRAY_EXTRACT_SIZE__(xs));
Array array_init(int *, size_t);

#define ARRAY_NEW(sz) array_new(__ARRAY_BIND_SIZE__(sz));
Array array_new(int *, size_t);

void array_log(const Array *);
ArrayOpResult array_push(Array *, int);

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

Array array_new(int *xs, size_t sz) {
  return (Array) { .cap = sz, .length = 0, .elems = xs };
}

Array array_init(int *xs, size_t sz) {
  return (Array) { .cap = sz, .length = sz, .elems = xs };
}

Array darray_new(int *xs, size_t init_sz) {
  return (Array) { .cap = init_sz, .length = 0, .elems = xs};
}

ArrayOpResult array_push(Array *xs, int x) {
  if (xs->length >= xs->cap) return FAILURE;
  xs->elems[xs->length++] = x;
  return SUCCESS;
}
#endif // ARRAY_IMPL
#endif // ARRAY_H_
