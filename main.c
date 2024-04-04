#include <stdio.h>

int len(char *str);

int main(void) {
  char *str = "hello";

  int l = len(str);
  printf("length is: %d\n", l);

  return 0;
}
