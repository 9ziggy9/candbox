#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

typedef enum {
  PANIC_GENERIC,
} panic_code_t;

#define PANIC(CODE)                                 \
  do {                                              \
    fprintf(stderr, "PANIC -> %s() :: ", __func__); \
    fprintf(stderr, ""#CODE"\n");                   \
    exit(CODE);                                     \
  } while(0);                                       \

void i_always_fail(void) { PANIC(PANIC_GENERIC); }

int main(void) {
  return 0;
}
