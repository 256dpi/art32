#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <art32/filter.h>

a32_filter_t *a32_filter_new(size_t num) {
  // allocate object
  a32_filter_t *s = (a32_filter_t *)malloc(sizeof(a32_filter_t));
  memset(s, 0, sizeof(a32_filter_t));

  // allocate history
  s->values = calloc(sizeof(double), num);
  s->sorted = calloc(sizeof(double), num);
  s->num = num;

  return s;
}

double a32_filter_update(a32_filter_t *s, double v) {
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

  // sort values
  memcpy(s->sorted, s->values, sizeof(double) * s->num);
  int i, element, j;
  for (i = 1; i < s->count; i++) {
    element = s->sorted[i];
    j = i - 1;
    while (j >= 0 && s->sorted[j] > element) {
      s->sorted[j + 1] = s->sorted[j];
      j = j - 1;
    }
    s->sorted[j + 1] = element;
  }

  // return middle number if odd
  if (s->count % 2 == 1) {
    return s->sorted[s->count / 2];
  }

  // otherwise, average
  return (s->sorted[s->count / 2 - 1] + s->sorted[s->count / 2]) / 2;
}

void a32_filter_free(a32_filter_t *s) {
  // free history
  free(s->values);
  free(s->sorted);

  // free object
  free(s);
}
