#ifndef A32_MATRIX_H
#define A32_MATRIX_H

#include <art32/vector.h>
#include <stddef.h>

/**
 * A matrix object.
 */
typedef struct {
  size_t rows;
  size_t cols;
  float **data;
} a32_matrix_t;

/**
 * Creates a new matrix.
 *
 * @param rows The row count.
 * @param cols The column count.
 * @return
 */
a32_matrix_t a32_matrix_new(size_t rows, size_t cols);

/**
 * Frees a matrix.
 */
void a32_matrix_free(a32_matrix_t);

/**
 * Calculates the right pseudo inverse.
 *
 * @return
 */
a32_matrix_t a32_matrix_right_pseudo_inverse(a32_matrix_t);

/**
 *  Prints the matrix using `printf`.
 */
void a32_matrix_print(a32_matrix_t);

#endif  // A32_MATRIX_H
