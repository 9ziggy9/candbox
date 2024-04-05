#include <stdio.h>
// BEGIN: custom logs
#define TRACE_FUNCTION_NAME printf("%s() :: ", __func__);
// END: custom logs
// BEGIN: simple C array macro example.
#define XS_LEN 4
#define PRINT_C_ARRAY(XS) print_c_array(XS, XS_LEN)
void print_c_array(int [], size_t length);
// END: simple C array macro example.
// BEGIN: augmented fixed length arrays
typedef enum {
  SUCCESS = 0,
  FAILURE,
} ArrayOpResult;

typedef struct {
  int *elements;
  size_t size;
  size_t length;
} Array;

void print_fixed_array(Array *);

// YO1 uses C standard built in variadic macro.
// YO2 uses GNU extension, it can be left empty.
#define YO1(fmt, ...) printf(fmt, __VA_ARGS__);
#define YO2(fmt, ...) printf(fmt, ##__VA_ARGS__); 

#define __ARRAY_EXTRACT_SIZE__(xs) xs, sizeof(xs) / sizeof(xs[0])
#define __ARRAY_BIND_SIZE__(sz) ((int [sz]){0}), sz

#define ARRAY_INIT(xs) array_init(__ARRAY_EXTRACT_SIZE__(xs));
Array array_init(int *, size_t); // stack allocatable

#define ARRAY_NEW(sz) array_new(__ARRAY_BIND_SIZE__(sz));
Array array_new(int *, size_t); // stack allocatable

ArrayOpResult array_push(Array *, int);
// END: augmented fixed length arrays

int main(void) {

  YO2("check 'em\n")

  Array xs = ARRAY_INIT(((int []){1,2,3,4,5}))
  print_fixed_array(&xs);

  Array ys = ARRAY_NEW(10);
  print_fixed_array(&ys);

  return 0;
}

// BEGIN: c_array impl
void print_c_array(int xs[], size_t length) {
  TRACE_FUNCTION_NAME;
  printf("{ ");
  for (size_t n = 0; n < length; n++) {
    printf("%d%s", xs[n], (n < length - 1) ? ", " : "\0");
  }
  printf(" }\n");
}
// END: c_array impl
// BEGIN: fixed array impl
Array array_new(int *xs, size_t sz) { // heap allocated
  return (Array) { .size = sz, .length = 0, .elements = xs };
}

Array array_init(int *xs, size_t sz) {
  return (Array) { .size = sz, .length = sz, .elements = xs };
}

ArrayOpResult array_push(Array *xs, int x) {
  if (xs->length >= xs->size) return FAILURE;
  xs->elements[xs->length++] = x;
  return SUCCESS;
}
void print_fixed_array(Array *xs) {
  TRACE_FUNCTION_NAME;
  printf("(length: %zu) :: (size: %zu) ", xs->length, xs->size);
  printf("{ ");
  if (xs->length == 0) printf("%s", "EMPTY");
  else
    for (size_t n = 0; n < xs->length; n++) {
      printf("%d%s", xs->elements[n], (n < xs->length - 1) ? ", " : "\0");
    }
  printf(" }\n");
}

// END: fixed array impl
