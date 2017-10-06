#include <stdlib.h>
#include <string.h>

#include <art32/smooth.h>

a32_smooth_t *a32_smooth_new(size_t num) {
  // allocate object
  a32_smooth_t *s = (a32_smooth_t *)malloc(sizeof(a32_smooth_t));
  memset(s, 0, sizeof(a32_smooth_t));

  // allocate history
  s->values = calloc(sizeof(double), num);
  s->num = num;

  return s;
}

double a32_smooth_update(a32_smooth_t *s, double v) {
  // subtract last reading
  s->total -= s->values[s->index];

  // save reading
  s->values[s->index] = v;

  // add reading
  s->total += v;

  // increment index
  s->index++;

  // check overflow
  if (s->index >= s->num) {
    s->index = 0;
  }

  // update cont
  if (s->count < s->num) {
    s->count++;
  }

  // return average
  return s->total / (double)s->count;
}

void smooth_free(a32_smooth_t *s) {
  // free history
  free(s->values);

  // free object
  free(s);
}
