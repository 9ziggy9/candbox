#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>

jmp_buf _runtime;

#define OK 0
#define ENTRY 1
#define LOOP 2

typedef enum {
  PANIC_GENERIC = 100,
  PANIC_TO_LOOP,
} panic_code_t;

#define LOG_YLW(STR) "\033[38;5;208m"STR"\033[0m"
#define LOG_GRN(STR) "\033[38;5;82m"STR"\033[0m"

#define PANIC(CODE)                                 \
  do {                                              \
    fprintf(stderr, "[%s]", LOG_YLW("PANIC"));      \
    fprintf(stderr, " -> %s() :: ", __func__);      \
    fprintf(stderr, ""#CODE"\n");                   \
    longjmp(_runtime, CODE);                        \
  } while(0);                                       \

#define RECOVER(S)                                                \
  do {                                                            \
    fprintf(stdout, "[%s]", LOG_GRN("INTERCEPTED"));              \
    fprintf(stdout, " -> %s() :: attempting recovery", __func__); \
    fprintf(stdout, " @ "#S"\n");                                 \
    longjmp(_runtime, S);                                         \
  } while(0);                                                     \

void panic_intercept(int run_status) {
  switch (run_status) {
  case PANIC_GENERIC:
    fprintf(stderr, "Panicking, encountered PANIC_GENERIC ...\n");
    RECOVER(ENTRY);
  case PANIC_TO_LOOP:
    fprintf(stderr, "Panicking, encountered PANIC_TO_LOOP ...\n");
    RECOVER(LOOP);
  }
}

int main(void) {
  int run_status = setjmp(_runtime);

  switch (run_status) {
  case OK:    init();      // initialization code here, fallthrough to _entry
  case ENTRY: goto _entry; // proceed as normal
  case LOOP:  goto _loop;  // second example
  default:    panic_intercept(run_status);
  }

_entry:
  printf("Program can proceed as normal.\n");
  printf("Preparing to panic AGAIN!\n");
  PANIC(PANIC_TO_LOOP);

_loop: {
    static int count = 10;
    printf("Now in loop...\n");
    if (--count > 0) goto _loop;
}

  return EXIT_SUCCESS;
}
