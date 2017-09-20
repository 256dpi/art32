#ifndef SMOOTH_H
#define SMOOTH_H

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
} smooth_t;

/**
 * Allocate a new smooth object.
 *
 * @param num - The stored history of values.
 * @return Pointer to a new smooth object.
 */
smooth_t *smooth_new(size_t num);

/**
 * Smooth the supplied value.
 *
 * @param s - The to be used smooth object.
 * @param v - The value to be smoothed.
 * @return The smoothed value.
 */
double smooth_update(smooth_t *s, double v);

/**
 * Free a previously allocated smooth object.
 *
 * @param s - The smooth object to be freed.
 */
void smooth_free(smooth_t *s);

#endif  // SMOOTH_H
