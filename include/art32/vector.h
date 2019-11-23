#ifndef A32_VECTOR_H
#define A32_VECTOR_H

#include <stddef.h>

/**
 * A vector object.
 */
typedef struct {
  size_t length;
  double *values;
} a32_vector_t;

/**
 * Creates a new vector using stack memory.
 *
 * @param name The variable name.
 * @param length The length.
 * @return A stack allocated vector.
 */
#define A32_VECTOR_MAKE(name, length)                      \
  double __##name##_values[(length)];                      \
  memset(__##name##_values, 0, sizeof(double) * (length)); \
  a32_vector_t name = a32_vector_use(__##name##_values, (length))

/**
 * Creates a new vector using heap memory.
 *
 * @param length The length.
 * @return A heap allocated vector.
 */
a32_vector_t a32_vector_new(size_t length);

/**
 * Frees a heap allocated vector.
 *
 * @param vec The vector.
 */
void a32_vector_free(a32_vector_t vec);

/**
 * Creates a vector from existing values.
 *
 * @param values
 * @param length
 * @return A stack allocated vector.
 */
a32_vector_t a32_vector_use(const double *values, size_t length);

/**
 * Copy values from one vector to another.
 *
 * @param dst The destination vector.
 * @param src The source vector.
 */
void a32_vector_copy(a32_vector_t dst, a32_vector_t src);

/**
 * Creates a view of another vector without copying.
 *
 * @param vec The vector.
 * @param offset The offset.
 * @param length The length.
 * @return The vector view.
 */

a32_vector_t a32_vector_view(a32_vector_t vec, size_t offset, size_t length);

/**
 * Find the biggest value.
 *
 * @param vec The vector.
 * @return The maximum.
 */
double a32_vector_max(a32_vector_t vec);

/**
 * Find the smallest value.
 *
 * @param vec The vector.
 * @return The minimum.
 */
double a32_vector_min(a32_vector_t vec);

/**
 * Calculates the magnitude of the vector.
 *
 * @param vec The vector.
 * @return The magnitude.
 */
double a32_vector_mag(a32_vector_t vec);

/**
 * Normalizes the vector to be a unit vector.
 *
 * @param vec The vector.
 */
void a32_vector_norm(a32_vector_t vec);

/**
 * Multiply two vectors.
 *
 * @param vec1 The first vector.
 * @param vec2 the second vector.
 */
void a32_vector_multiply(a32_vector_t vec1, a32_vector_t vec2);

/**
 * Prints the vector using `printf`.
 *
 * @param vec The vector.
 */
void a32_vector_print(a32_vector_t vec);

#endif  // A32_VECTOR_H
