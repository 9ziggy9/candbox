#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define XS_LEN 4
#define PRINT_C_ARRAY(XS) print_c_array(XS, XS_LEN)

void print_c_array(int xs[], size_t length);
size_t len(const char *str);

typedef struct {
  int *elements;
  size_t size;
  size_t length;
} Array;

#define FUNCTION_NAME printf("%s() :: ", __func__);

void print_fixed_array(Array xs) {
  FUNCTION_NAME
  printf("%s\n", (NULL == (0) ? "true" : "false"));
  printf("{ ");
  for (size_t n = 0; n < xs.length; n++) {
    printf("%d%s", xs.elements[n], (n < xs.length - 1) ? ", " : "\0");
  }
  printf(" }\n");
}

typedef enum {
  SUCCESS = 0,
  FAILURE,
} ArrayOpResult ;

// new
Array array_new(size_t size) {
  return (Array) {
    .size     = size,
    .length   = 0,
    .elements = (int *) calloc(size, sizeof(int))
  };
}

// push
// DEFINITION: (*xs).length == xs->length
ArrayOpResult array_push(Array *xs, int x) {
  if (xs->length >= xs->size) return FAILURE;
  xs->elements[xs->length++] = x;
  return SUCCESS;
}

// fns and stuff
// macros

// dynamic arrays <-- realloc

int main(void) {
  print_c_array((int []){1,2,3,4}, XS_LEN); // XS_LEN MUST BE KNOWN
  PRINT_C_ARRAY(((int []){1,2,3,4})); // macro hack

  Array xs = array_new(4);
  array_push(&xs, 1);
  array_push(&xs, 2);
  array_push(&xs, 3);
  array_push(&xs, 4);
  print_fixed_array(xs);
  return 0;
}

void print_c_array(int xs[], size_t length) {
  FUNCTION_NAME
  printf("{ ");
  for (size_t n = 0; n < length; n++) {
    printf("%d%s", xs[n], (n < length - 1) ? ", " : "\0");
  }
  printf(" }\n");
}
