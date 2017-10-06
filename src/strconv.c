#include <stdio.h>

const char* a32_dtostr(double num) {
  static char str[16] = {0};
  snprintf(str, 16, "%.4f", num);
  return str;
}

const char* a32_ltostr(long num) {
  static char str[16] = {0};
  snprintf(str, 16, "%ld", num);
  return str;
}
