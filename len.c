#include <stdio.h>

size_t len(const char *str) {
  size_t l = 0;
  while(*(str++) != '\0') l++;
  return l;
}
