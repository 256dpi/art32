#include <stdlib.h>
#include <string.h>

#include <art32/smooth.h>

a32_smooth_t *a32_smooth_new(size_t num) {
  // check history size (a zero sized history has no values to average)
  if (num == 0) {
    return NULL;
  }

  // allocate object
  a32_smooth_t *s = (a32_smooth_t *)calloc(1, sizeof(a32_smooth_t));
  if (s == NULL) {
    return NULL;
  }

  // allocate history
  s->values = calloc(num, sizeof(float));
  s->num = num;

  // check history
  if (s->values == NULL) {
    a32_smooth_free(s);
    return NULL;
  }

  return s;
}

float a32_smooth_update(a32_smooth_t *s, float v) {
  // subtract last reading
  s->total -= s->values[s->index];

  // save reading
  s->values[s->index] = v;

  // add current reading
  s->total += v;

  // increment index and wrap around
  s->index++;
  if (s->index >= s->num) {
    s->index = 0;
  }

  // update count
  if (s->count < s->num) {
    s->count++;
  }

  // update min
  s->min = s->values[0];
  for (size_t i = 1; i < s->count; i++) {
    if (s->values[i] < s->min) {
      s->min = s->values[i];
    }
  }

  // update max
  s->max = s->values[0];
  for (size_t i = 1; i < s->count; i++) {
    if (s->values[i] > s->max) {
      s->max = s->values[i];
    }
  }

  // return average
  return s->total / (float)s->count;
}

void a32_smooth_free(a32_smooth_t *s) {
  // free history
  free(s->values);

  // free object
  free(s);
}
