#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <art32/filter.h>

a32_filter_t *a32_filter_new(size_t num) {
  // check history size (a zero sized history has no values to select from)
  if (num == 0) {
    return NULL;
  }

  // allocate object
  a32_filter_t *f = (a32_filter_t *)calloc(1, sizeof(a32_filter_t));
  if (f == NULL) {
    return NULL;
  }

  // allocate history
  f->values = calloc(num, sizeof(double));
  f->sorted = calloc(num, sizeof(double));
  f->num = num;

  // check history
  if (f->values == NULL || f->sorted == NULL) {
    a32_filter_free(f);
    return NULL;
  }

  return f;
}

// Finds the first index in the sorted values that is not ordered before the value. This
// is the position of the value itself if it is present and its insertion point if not.
static size_t a32_filter_search(a32_filter_t *f, double v) {
  size_t lo = 0;
  size_t hi = f->count;

  while (lo < hi) {
    size_t mid = lo + (hi - lo) / 2;
    if (f->sorted[mid] < v) {
      lo = mid + 1;
    } else {
      hi = mid;
    }
  }

  return lo;
}

double a32_filter_update(a32_filter_t *f, double v) {
  // The sorted values are maintained across updates instead of being re-sorted from
  // scratch: the value that is about to be overwritten is removed and the new reading is
  // inserted at its position.

  // remove the overwritten reading once the history is full
  if (f->count == f->num) {
    size_t pos = a32_filter_search(f, f->values[f->index]);
    memmove(f->sorted + pos, f->sorted + pos + 1, sizeof(double) * (f->count - pos - 1));
    f->count--;
  }

  // save reading
  f->values[f->index] = v;

  // increment index and wrap around
  f->index++;
  if (f->index >= f->num) {
    f->index = 0;
  }

  // insert reading into the sorted values
  size_t pos = a32_filter_search(f, v);
  memmove(f->sorted + pos + 1, f->sorted + pos, sizeof(double) * (f->count - pos));
  f->sorted[pos] = v;
  f->count++;

  // return middle number if odd
  if (f->count % 2 == 1) {
    return f->sorted[f->count / 2];
  }

  // otherwise, average
  return (f->sorted[f->count / 2 - 1] + f->sorted[f->count / 2]) / 2;
}

void a32_filter_free(a32_filter_t *f) {
  // handle null object
  if (f == NULL) {
    return;
  }

  // free history
  free(f->values);
  free(f->sorted);

  // free object
  free(f);
}
