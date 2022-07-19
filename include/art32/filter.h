#ifndef A32_FILTER_H
#define A32_FILTER_H

#include <stddef.h>

/**
 * A filter object.
 */
typedef struct {
  size_t num;
  double *values;
  double *sorted;
  size_t index;
  size_t count;
} a32_filter_t;

/**
 * Allocate a new filter object.
 *
 * @param num The stored history of values (should be odd).
 * @return Pointer to a new filter object.
 */
a32_filter_t *a32_filter_new(size_t num);

/**
 * Adds the provided value to the history and returns the median value.
 *
 * @param s The filter object to be used.
 * @param v The value to be filtered.
 * @return The filtered value.
 */
double a32_filter_update(a32_filter_t *s, double v);

/**
 * Free a previously allocated filter object.
 *
 * @param s The filter object to be freed.
 */
void a32_filter_free(a32_filter_t *s);

#endif  // A32_FILTER_H
