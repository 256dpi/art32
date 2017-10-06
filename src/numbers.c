double a32_constrain_d(double num, double min, double max) { return num > max ? max : num < min ? min : num; }

float a32_constrain_f(float num, float min, float max) { return (float)a32_constrain_d(num, min, max); }

long a32_constrain_l(long num, long min, long max) { return num > max ? max : num < min ? min : num; }

int a32_constrain_i(int num, int min, int max) { return (int)a32_constrain_l(num, min, max); }

double a32_map_d(double num, double in_min, double in_max, double out_min, double out_max) {
  return (num - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float a32_map_f(float num, float in_min, float in_max, float out_min, float out_max) {
  return (float)a32_map_d(num, in_min, in_max, out_min, out_max);
}

long a32_map_l(long num, long in_min, long in_max, long out_min, long out_max) {
  return (num - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int a32_map_i(int num, int in_min, int in_max, int out_min, int out_max) {
  return (int)a32_map_l(num, in_min, in_max, out_min, out_max);
}
