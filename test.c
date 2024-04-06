#include <stdio.h>

int main(void) {
  float x = 2.75;
  printf("x is %x\n", *(int *)&x);
  return 0;
}
