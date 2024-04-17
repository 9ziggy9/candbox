#include <stdio.h>

int main(void) {
  const char *abc = "abcdefghijklmnop";
  while (*abc != '\0') printf("ASCII: %d\n", *abc++);
  return 0;
}
