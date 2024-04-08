#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define PRINT_BINARY(n)                                    \
  do {                                                     \
    uint64_t num = n;                                      \
    uint64_t mask = ((uint64_t) 1) << (8*sizeof(num) - 1); \
    bool started = false;                                  \
    printf("0b");                                          \
    while (mask) {                                         \
      if (num & mask) {                                    \
        started = true;                                    \
        printf("1");                                       \
      } else if (started) {                                \
        printf("0");                                       \
      }                                                    \
      mask >>= 1;                                          \
    }                                                      \
    printf("\n");                                          \
  } while (0);                                             \

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t alpha;
} RGBA;

RGBA rgba_from_hex(uint32_t hex) {
  return (RGBA) {
    .red    = (hex >> 24) & 0xFF,
    .green  = (hex >> 16) & 0xFF,
    .blue   = (hex >> 8)  & 0xFF,
    .alpha  = (hex >> 0)  & 0xFF,
  };
}

int main(void) {
  RGBA color = rgba_from_hex(0xFFFFFFFF);
  return 0;
}
