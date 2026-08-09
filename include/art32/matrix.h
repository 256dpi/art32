#ifndef A32_MATRIX_H
#define A32_MATRIX_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "vector.h"

/**
 * A matrix object.
 */
typedef struct {
  size_t rows;
  size_t cols;
  double* values;
} a32_matrix_t;

/**
 * Short-hand helper for accessing matrix elements.
 *
 * Note: This macro evaluates its arguments more than once, therefore arguments must not
 * have side effects.
 */
#define A32_MAT(mat, row, col) (mat).values[(row) * (mat).cols + (col)]

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
 * Note: The returned matrix has a NULL `values` pointer if the allocation failed.
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
 * Create a matrix from existing values.
 *
 * @param values The values.
 * @param rows The row count.
 * @param cols The column count.
 * @return The stack allocated matrix.
 */
a32_matrix_t a32_matrix_use(const double* values, size_t rows, size_t cols);

/**
 * Copy values from one matrix to another.
 *
 * @param dst The destination matrix.
 * @param src The source matrix.
 */
void a32_matrix_copy(a32_matrix_t dst, a32_matrix_t src);

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
 * Add a matrix to a matrix.
 *
 * @param mat The matrix.
 * @param add The addend.
 */
void a32_matrix_add(a32_matrix_t mat, a32_matrix_t add);

/**
 * Subtract a matrix from a matrix.
 *
 * @param mat The matrix.
 * @param sub The subtrahend.
 */
void a32_matrix_subtract(a32_matrix_t mat, a32_matrix_t sub);

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
 */
void a32_matrix_multiply_scalar(a32_matrix_t mat, double scalar);

/**
 * Multiply a vector with a matrix.
 *
 * @param vec The vector.
 * @param mat The matrix.
 * @return The product.
 */
a32_vector_t a32_vector_multiply_matrix(a32_vector_t vec, a32_matrix_t mat);

/**
 * Calculates the inverse of a square matrix using LU decomposition.
 *
 * A singular matrix has no inverse. In that case `ok` is set to false and a zero matrix
 * is returned. The result must be freed in either case.
 *
 * @param mat The matrix.
 * @param ok Receives whether the matrix could be inverted. May be NULL.
 * @return The inverted matrix.
 */
a32_matrix_t a32_matrix_invert(a32_matrix_t mat, bool* ok);

/**
 * Calculates the pseudo inverse of the matrix.
 *
 * Beyond fully zero rows and columns, the matrix must have full rank. For a rank
 * deficient matrix `ok` is set to false and the result is undefined. The result must be
 * freed in either case.
 *
 * @param mat The matrix.
 * @param ok Receives whether the pseudo inverse could be calculated. May be NULL.
 * @return The calculated matrix.
 */
a32_matrix_t a32_matrix_pseudo_inverse(a32_matrix_t mat, bool* ok);

/**
 *  Prints the matrix using `printf`.
 *
 * @param mat The matrix.
 */
void a32_matrix_print(a32_matrix_t mat);

#endif  // A32_MATRIX_H
