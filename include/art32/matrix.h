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
  double **values;
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
 * Create a matrix from a two dimensional array.
 *
 * @param values The values.
 * @param rows The row count.
 * @param cols The column count.
 * @return The allocated matrix.
 */
a32_matrix_t a32_matrix_use(const double *values, size_t rows, size_t cols);

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
 * Set row from vector.
 *
 * @param mat The matrix.
 * @param row The row.
 * @param vec The vector.
 */
void a32_matrix_set_row(a32_matrix_t mat, size_t row, a32_vector_t vec);

/**
 * Set column from vector.
 *
 * @param mat The matrix.
 * @param col The column.
 * @param vec The vector.
 */
void a32_matrix_set_col(a32_matrix_t mat, size_t col, a32_vector_t vec);

/**
 * Transpose matrix, flip over its diagonal.
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
 * @return The calculated matrix.
 */
a32_matrix_t a32_matrix_multiply(a32_matrix_t mat1, a32_matrix_t mat2);

/**
 * Calculates the pseudo inverse of the matrix.
 *
 * @return The calculated matrix.
 */
a32_matrix_t a32_matrix_pseudo_inverse(a32_matrix_t mat);

/**
 *  Prints the matrix using `printf`.
 */
void a32_matrix_print(a32_matrix_t);

#endif  // A32_MATRIX_H
