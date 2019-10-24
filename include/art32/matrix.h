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
 * @return The allocated matrix.
 */
a32_matrix_t a32_matrix_new(size_t rows, size_t cols);

/**
 * Frees a matrix.
 */
void a32_matrix_free(a32_matrix_t);

/**
 * Copy matrix.
 *
 * @param mat The matrix.
 * @return The copied matrix.
 */
a32_matrix_t a32_matrix_copy(a32_matrix_t mat);

/**
 * Transpose matrix (flip over its diagonal).
 *
 * @param mat The matrix.
 * @return The transposed matrix.
 */
a32_matrix_t a32_matrix_transpose(a32_matrix_t mat);

/**
 * Calculate the product of two matrices.
 *
 * @param mat1 The first matrix.
 * @param mat2 The second matrix.
 * @return The calculate matrix.
 */
a32_matrix_t a32_matrix_product(a32_matrix_t mat1, a32_matrix_t mat2);

/**
 * Calculates the right pseudo inverse.
 *
 * @return The calculate matrix.
 */
a32_matrix_t a32_matrix_right_pseudo_inverse(a32_matrix_t);

/**
 *  Prints the matrix using `printf`.
 */
void a32_matrix_print(a32_matrix_t);

#endif  // A32_MATRIX_H
