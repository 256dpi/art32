#include <stdlib.h>
#include <string.h>

#include <art32/smooth.h>

smooth_t *smooth_new(size_t num) {
  // allocate object
  smooth_t *s = (smooth_t *)malloc(sizeof(smooth_t));
  memset(s, 0, sizeof(smooth_t));

  // allocate history
  s->values = calloc(sizeof(double), num);
  s->num = num;

  return s;
}

double smooth_update(smooth_t *s, double v) {
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

void smooth_free(smooth_t* s) {
  // free history
  free(s->values);

  // free object
  free(s);
}
