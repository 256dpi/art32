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
  // save reading
  s->values[s->index] = v;

  // increment index and wrap around
  s->index++;
  if (s->index >= s->num) {
    s->index = 0;
  }

  // update count
  if (s->count < s->num) {
    s->count++;
  }

  // recompute total, min and max over the history (carrying the total across updates
  // would accumulate the rounding error of every reading ever added)
  s->total = s->values[0];
  s->min = s->values[0];
  s->max = s->values[0];
  for (size_t i = 1; i < s->count; i++) {
    float value = s->values[i];
    s->total += value;
    if (value < s->min) {
      s->min = value;
    }
    if (value > s->max) {
      s->max = value;
    }
  }

  // return average
  return s->total / (float)s->count;
}

void a32_smooth_free(a32_smooth_t *s) {
  // handle null object
  if (s == NULL) {
    return;
  }

  // free history
  free(s->values);

  // free object
  free(s);
}
