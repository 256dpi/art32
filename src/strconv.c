#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* a32_trim(char* str) {
  // skip whitespace
  while (str[0] == ' ') {
    str++;
  }

  // clear whitespace
  size_t len = strlen(str);
  while (str[len - 1] == ' ') {
    str[len - 1] = '\0';
    len--;
  }

  return str;
}

const char* a32_i2str(int32_t num) {
  static char str[32] = {0};
  snprintf(str, 32, "%d", num);
  return str;
}

const char* a32_l2str(int64_t num) {
  static char str[32] = {0};
  snprintf(str, 32, "%lld", num);
  return str;
}

const char* a32_f2str(float num) {
  static char str[32] = {0};
  snprintf(str, 32, "%.4f", num);
  return str;
}

const char* a32_d2str(double num) {
  static char str[32] = {0};
  snprintf(str, 32, "%.4f", num);
  return str;
}

int32_t a32_str2i(const char* str) { return strtol(str, NULL, 10); }

int64_t a32_str2l(const char* str) { return strtoll(str, NULL, 10); }

float a32_str2f(const char* str) { return strtof(str, NULL); }

double a32_str2d(const char* str) { return strtod(str, NULL); }
