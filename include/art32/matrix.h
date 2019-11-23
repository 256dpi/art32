#ifndef A32_MATRIX_H
#define A32_MATRIX_H

#include <stddef.h>

#include "vector.h"

/**
 * A matrix object.
 */
typedef struct {
  size_t rows;
  size_t cols;
  double *values;
} a32_matrix_t;

/**
 * Helper for accessing matrix elements.
 */
#define A32_MAT(mat, row, col) mat.values[row * mat.cols + col]

/**
 * Creates a new matrix using stack memory.
 *
 * @param name The variable name.
 * @param rows The row count.
 * @param cols The column count.
 * @return A stack allocated matrix.
 */
#define A32_MATRIX_MAKE(name, rows, columns)                         \
  double __##name##_values[(rows) * (columns)];                      \
  memset(__##name##_values, 0, sizeof(double) * (rows) * (columns)); \
  a32_matrix_t name = a32_matrix_use(__##name##_values, (rows), (columns))

/**
 * Creates a new matrix using heap memory.
 *
 * @param rows The row count.
 * @param cols The column count.
 * @return The allocated matrix.
 */
a32_matrix_t a32_matrix_new(size_t rows, size_t cols);

/**
 * Frees a heap allocated matrix.
 *
 * @param mat The matrix.
 */
void a32_matrix_free(a32_matrix_t mat);

/**
 * Create a heap allocated matrix from existing values.
 *
 * @param values The values.
 * @param rows The row count.
 * @param cols The column count.
 * @return The allocated matrix.
 */
a32_matrix_t a32_matrix_use(const double *values, size_t rows, size_t cols);

/**
 * Creates a heap allocated copy of the matrix.
 *
 * @param mat The matrix.
 * @return The copied matrix.
 */
a32_matrix_t a32_matrix_copy(a32_matrix_t mat);

/**
 * Get a row from the matrix.
 * @param mat The matrix.
 * @param row The row.
 * @return The extracted vector.
 */
a32_vector_t a32_matrix_get_row(a32_matrix_t mat, size_t row);

/**
 * Get a column from the matrix.
 * @param mat The matrix.
 * @param col The column.
 * @return The extracted vector.
 */
a32_vector_t a32_matrix_get_col(a32_matrix_t mat, size_t col);

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
 * Add two matrices.
 *
 * @param mat1 The first matrix.
 * @param mat2 The second matrix.
 * @return The sum of both matrices.
 */
a32_matrix_t a32_matrix_add(a32_matrix_t mat1, a32_matrix_t mat2);

/**
 * Subtract two matrices.
 *
 * @param mat1 The first matrix.
 * @param mat2 The second matrix.
 * @return The difference of both matrices.
 */
a32_matrix_t a32_matrix_subtract(a32_matrix_t mat1, a32_matrix_t mat2);

/**
 * Calculate the product of two matrices.
 *
 * @param mat1 The first matrix.
 * @param mat2 The second matrix.
 * @return The calculated matrix.
 */
a32_matrix_t a32_matrix_multiply(a32_matrix_t mat1, a32_matrix_t mat2);

/**
 * Calculates the product of a matrix with a scalar.
 *
 * @param mat The matrix.
 * @param scalar The scalar.
 * @return The calculated matrix.
 */
a32_matrix_t a32_matrix_multiply_scalar(a32_matrix_t mat, double scalar);

/**
 * Multiply a vector with a matrix.
 *
 * @param vec The vector.
 * @param mat The matrix.
 * @return The product.
 */
a32_vector_t a32_vector_multiply_matrix(a32_vector_t vec, a32_matrix_t mat);

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
