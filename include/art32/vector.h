#ifndef A32_VECTOR_H
#define A32_VECTOR_H

#include <stddef.h>

/**
 * A vector object.
 */
typedef struct {
  size_t len;
  float *data;
} a32_vector_t;

/**
 * Creates a new vector.
 *
 * @param length The length.
 * @return
 */
a32_vector_t a32_vector_new(size_t length);

/**
 * Frees a vector.
 *
 * @param vec The vector.
 */
void a32_vector_free(a32_vector_t vec);

/**
 * Finds the biggest number.
 *
 * @param vec The vector.
 * @return The maximum.
 */
float a32_vector_max(a32_vector_t vec);

/**
 * Finds the smallest number.
 *
 * @param vec The vector.
 * @return The minimum.
 */
float a32_vector_min(a32_vector_t vec);

#endif  // A32_VECTOR_H
