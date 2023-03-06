#include "art32/numbers.h"

int32_t a32_constrain_i(int32_t num, int32_t min, int32_t max) { return (int32_t)a32_constrain_l(num, min, max); }

int64_t a32_constrain_l(int64_t num, int64_t min, int64_t max) { return num > max ? max : num < min ? min : num; }

float a32_constrain_f(float num, float min, float max) { return (float)a32_constrain_d(num, min, max); }

double a32_constrain_d(double num, double min, double max) { return num > max ? max : num < min ? min : num; }

int32_t a32_map_i(int32_t num, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
  return (int32_t)a32_map_l(num, in_min, in_max, out_min, out_max);
}

int64_t a32_map_l(int64_t num, int64_t in_min, int64_t in_max, int64_t out_min, int64_t out_max) {
  return (num - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float a32_map_f(float num, float in_min, float in_max, float out_min, float out_max) {
  return (float)a32_map_d(num, in_min, in_max, out_min, out_max);
}

double a32_map_d(double num, double in_min, double in_max, double out_min, double out_max) {
  return (num - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int32_t a32_safe_map_i(int32_t num, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
  return (int32_t)a32_safe_map_l(num, in_min, in_max, out_min, out_max);
}

int64_t a32_safe_map_l(int64_t num, int64_t in_min, int64_t in_max, int64_t out_min, int64_t out_max) {
  return a32_constrain_l(a32_map_l(num, in_min, in_max, out_min, out_max), out_min, out_max);
}

float a32_safe_map_f(float num, float in_min, float in_max, float out_min, float out_max) {
  return (float)a32_safe_map_d(num, in_min, in_max, out_min, out_max);
}

double a32_safe_map_d(double num, double in_min, double in_max, double out_min, double out_max) {
  return a32_constrain_d(a32_map_d(num, in_min, in_max, out_min, out_max), out_min, out_max);
}
