#include <stdio.h>

const char* a32_dtostr(double num) {
  static char position_str[16] = {0};
  snprintf(position_str, 16, "%.4f", num);
  return position_str;
}
