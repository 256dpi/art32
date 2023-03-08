#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <art32/filter.h>

a32_filter_t *a32_filter_new(size_t num) {
  // allocate object
  a32_filter_t *f = (a32_filter_t *)malloc(sizeof(a32_filter_t));
  memset(f, 0, sizeof(a32_filter_t));

  // allocate history
  f->values = calloc(sizeof(double), num);
  f->sorted = calloc(sizeof(double), num);
  f->num = num;

  return f;
}

double a32_filter_update(a32_filter_t *f, double v) {
  // save reading
  f->values[f->index] = v;

  // increment index and wrap around
  f->index++;
  if (f->index >= f->num) {
    f->index = 0;
  }

  // update count
  if (f->count < f->num) {
    f->count++;
  }

  // sort values
  memcpy(f->sorted, f->values, sizeof(double) * f->num);
  size_t i, j;
  double element;
  for (i = 1; i < f->count; i++) {
    element = f->sorted[i];
    j = i - 1;
    while (f->sorted[j] > element) {
      f->sorted[j + 1] = f->sorted[j];
      j = j - 1;
    }
    f->sorted[j + 1] = element;
  }

  // return middle number if odd
  if (f->count % 2 == 1) {
    return f->sorted[f->count / 2];
  }

  // otherwise, average
  return (f->sorted[f->count / 2 - 1] + f->sorted[f->count / 2]) / 2;
}

void a32_filter_free(a32_filter_t *f) {
  // free history
  free(f->values);
  free(f->sorted);

  // free object
  free(f);
}
