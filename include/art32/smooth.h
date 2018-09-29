#ifndef A32_SMOOTH_H
#define A32_SMOOTH_H

#include <stddef.h>

/**
 * A smooth object.
 */
typedef struct {
  size_t num;
  double *values;
  size_t index;
  double total;
  size_t count;
  double min;
  double max;
} a32_smooth_t;

/**
 * Allocate a new smooth object.
 *
 * @param num The stored history of values.
 * @return Pointer to a new smooth object.
 */
a32_smooth_t *a32_smooth_new(size_t num);

/**
 * Adds the provided value to the history and returns the mean value.
 *
 * @param s The smooth object to be used.
 * @param v The value to be smoothed.
 * @return The smoothed value.
 */
double a32_smooth_update(a32_smooth_t *s, double v);

/**
 * Free a previously allocated smooth object.
 *
 * @param s The smooth object to be freed.
 */
void a32_smooth_free(a32_smooth_t *s);

#endif  // A32_SMOOTH_H
