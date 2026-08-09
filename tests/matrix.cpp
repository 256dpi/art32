#include <gtest/gtest.h>

extern "C" {
#include <art32/matrix.h>
}

// Expected values are taken from `mat.py` (numpy).

static void expect_matrix(a32_matrix_t mat, size_t rows, size_t cols, const double* expected, double delta = 1e-3) {
  ASSERT_EQ(mat.rows, rows);
  ASSERT_EQ(mat.cols, cols);
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      EXPECT_NEAR(A32_MAT(mat, r, c), expected[r * cols + c], delta) << "at " << r << "," << c;
    }
  }
}

TEST(Matrix, Transpose) {
  double values[2][3] = {{1, 2, 3}, {4, 5, 6}};
  a32_matrix_t mat = a32_matrix_use((double*)values, 2, 3);
  a32_matrix_t res = a32_matrix_transpose(mat);
  double expected[3][2] = {{1, 4}, {2, 5}, {3, 6}};
  expect_matrix(res, 3, 2, (double*)expected);
  a32_matrix_free(res);
}

TEST(Matrix, AddSubtract) {
  double a[2][2] = {{1, 2}, {3, 4}};
  double b[2][2] = {{5, 6}, {7, 8}};
  a32_matrix_t mat = a32_matrix_use((double*)a, 2, 2);
  a32_matrix_t add = a32_matrix_use((double*)b, 2, 2);

  a32_matrix_add(mat, add);
  double sum[2][2] = {{6, 8}, {10, 12}};
  expect_matrix(mat, 2, 2, (double*)sum);

  a32_matrix_subtract(mat, add);
  double orig[2][2] = {{1, 2}, {3, 4}};
  expect_matrix(mat, 2, 2, (double*)orig);
}

TEST(Matrix, Multiply) {
  double a[2][3] = {{1, 2, 3}, {4, 5, 6}};
  double b[3][2] = {{7, 8}, {9, 10}, {11, 12}};
  a32_matrix_t mat1 = a32_matrix_use((double*)a, 2, 3);
  a32_matrix_t mat2 = a32_matrix_use((double*)b, 3, 2);
  a32_matrix_t res = a32_matrix_multiply(mat1, mat2);
  double expected[2][2] = {{58, 64}, {139, 154}};
  expect_matrix(res, 2, 2, (double*)expected);
  a32_matrix_free(res);
}

TEST(Matrix, MultiplyScalar) {
  double values[2][2] = {{1, 2}, {3, 4}};
  a32_matrix_t mat = a32_matrix_use((double*)values, 2, 2);
  a32_matrix_multiply_scalar(mat, 2.5);
  double expected[2][2] = {{2.5, 5}, {7.5, 10}};
  expect_matrix(mat, 2, 2, (double*)expected);
}

TEST(Matrix, RowsAndCols) {
  double values[2][3] = {{1, 2, 3}, {4, 5, 6}};
  a32_matrix_t mat = a32_matrix_use((double*)values, 2, 3);

  a32_vector_t row = a32_matrix_get_row(mat, 1);
  ASSERT_EQ(row.length, (size_t)3);
  ASSERT_EQ(row.values[0], 4);
  ASSERT_EQ(row.values[2], 6);

  a32_vector_t col = a32_matrix_get_col(mat, 2);
  ASSERT_EQ(col.length, (size_t)2);
  ASSERT_EQ(col.values[0], 3);
  ASSERT_EQ(col.values[1], 6);

  double rep[3] = {7, 8, 9};
  a32_matrix_set_row(mat, 0, a32_vector_use(rep, 3));
  ASSERT_EQ(A32_MAT(mat, 0, 0), 7);
  ASSERT_EQ(A32_MAT(mat, 0, 2), 9);

  double repc[2] = {0, 1};
  a32_matrix_set_col(mat, 1, a32_vector_use(repc, 2));
  ASSERT_EQ(A32_MAT(mat, 0, 1), 0);
  ASSERT_EQ(A32_MAT(mat, 1, 1), 1);

  a32_vector_free(row);
  a32_vector_free(col);
}

TEST(Matrix, VectorMultiplyMatrix) {
  double values[2][3] = {{1, 2, 3}, {4, 5, 6}};
  a32_matrix_t mat = a32_matrix_use((double*)values, 2, 3);
  double v[3] = {1, 2, 3};
  a32_vector_t res = a32_vector_multiply_matrix(a32_vector_use(v, 3), mat);
  ASSERT_EQ(res.length, (size_t)2);
  ASSERT_DOUBLE_EQ(res.values[0], 14);
  ASSERT_DOUBLE_EQ(res.values[1], 32);
  a32_vector_free(res);
}

TEST(Matrix, Invert) {
  double values[3][3] = {{2, 0, 1}, {1, 3, 2}, {1, 1, 4}};
  a32_matrix_t mat = a32_matrix_use((double*)values, 3, 3);
  a32_matrix_t inv = a32_matrix_invert(mat, nullptr);

  // the product must be the identity
  a32_matrix_t prod = a32_matrix_multiply(mat, inv);
  double identity[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  expect_matrix(prod, 3, 3, (double*)identity, 1e-12);

  a32_matrix_free(inv);
  a32_matrix_free(prod);
}

// Regression: `lu_back_substitute` used int8_t indices, so inversion silently produced
// garbage for matrices with more than 127 rows.
TEST(Matrix, InvertLarge) {
  const size_t n = 130;
  a32_matrix_t mat = a32_matrix_new(n, n);
  for (size_t r = 0; r < n; r++) {
    for (size_t c = 0; c < n; c++) {
      A32_MAT(mat, r, c) = (r == c) ? 2.0 : 0.1;
    }
  }

  a32_matrix_t inv = a32_matrix_invert(mat, nullptr);
  a32_matrix_t prod = a32_matrix_multiply(mat, inv);

  for (size_t r = 0; r < n; r++) {
    for (size_t c = 0; c < n; c++) {
      ASSERT_NEAR(A32_MAT(prod, r, c), (r == c) ? 1.0 : 0.0, 1e-9) << "at " << r << "," << c;
    }
  }

  a32_matrix_free(mat);
  a32_matrix_free(inv);
  a32_matrix_free(prod);
}

TEST(Matrix, PseudoInverse2x2) {
  double values[2][2] = {{0.1, 0.2}, {0.3, 0.4}};
  a32_matrix_t mat = a32_matrix_use((double*)values, 2, 2);
  a32_matrix_t res = a32_matrix_pseudo_inverse(mat, nullptr);
  double expected[2][2] = {{-20, 10}, {15, -5}};
  expect_matrix(res, 2, 2, (double*)expected);
  a32_matrix_free(res);
}

TEST(Matrix, PseudoInverse2x3) {
  double values[2][3] = {{0.1, 0.2, 0.1}, {0.3, 0.4, 0.1}};
  a32_matrix_t mat = a32_matrix_use((double*)values, 2, 3);
  a32_matrix_t res = a32_matrix_pseudo_inverse(mat, nullptr);
  double expected[3][2] = {{-8.3333, 5}, {3.3333, 0}, {11.6667, -5}};
  expect_matrix(res, 3, 2, (double*)expected);
  a32_matrix_free(res);
}

TEST(Matrix, PseudoInverse3x3) {
  double values[3][3] = {{0.1, 0.2, 0.1}, {0.3, 0.4, 0.1}, {0.1, 0.2, 0.3}};
  a32_matrix_t mat = a32_matrix_use((double*)values, 3, 3);
  a32_matrix_t res = a32_matrix_pseudo_inverse(mat, nullptr);
  double expected[3][3] = {{-25, 10, 5}, {20, -5, -5}, {-5, 0, 5}};
  expect_matrix(res, 3, 3, (double*)expected);
  a32_matrix_free(res);
}

TEST(Matrix, PseudoInverse3x5WithZeroRowAndCols) {
  double values[3][5] = {
      {0.0, 0.1, 0.2, 0.0, 0.1},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.3, 0.4, 0.0, 0.1},
  };
  a32_matrix_t mat = a32_matrix_use((double*)values, 3, 5);
  a32_matrix_t res = a32_matrix_pseudo_inverse(mat, nullptr);
  double expected[5][3] = {
      {0, 0, 0}, {-8.3333, 0, 5}, {3.3333, 0, 0}, {0, 0, 0}, {11.6667, 0, -5},
  };
  expect_matrix(res, 5, 3, (double*)expected);
  a32_matrix_free(res);
}

TEST(Matrix, PseudoInverse5x3WithZeroRowAndCols) {
  double values[5][3] = {
      {0.0, 0.0, 0.0}, {0.1, 0.0, 0.3}, {0.2, 0.0, 0.4}, {0.0, 0.0, 0.0}, {0.1, 0.0, 0.1},
  };
  a32_matrix_t mat = a32_matrix_use((double*)values, 5, 3);
  a32_matrix_t res = a32_matrix_pseudo_inverse(mat, nullptr);
  double expected[3][5] = {
      {0, -8.3333, 3.3333, 0, 11.6667},
      {0, 0, 0, 0, 0},
      {0, 5, 0, 0, -5},
  };
  expect_matrix(res, 3, 5, (double*)expected);
  a32_matrix_free(res);
}

TEST(Matrix, PseudoInverse6x6) {
  double values[6][6] = {
      {+0.000, +0.000, +1.000, +0.000, +1.000, +0.000},   {+1.000, +0.000, +0.000, +1.000, +0.000, +0.000},
      {+0.000, +1.000, +0.000, +0.000, +0.000, +1.000},   {-0.000, -60.000, -10.000, +0.000, +0.000, -43.000},
      {+0.000, -20.000, +0.000, +0.000, +0.000, +30.000}, {+100.000, +0.000, +142.000, -95.000, -141.000, +0.000},
  };
  a32_matrix_t mat = a32_matrix_use((double*)values, 6, 6);
  a32_matrix_t res = a32_matrix_pseudo_inverse(mat, nullptr);
  double expected[6][6] = {
      {0.7231, 0.4872, 7.7208, 0.1451, -0.0493, 0.0051},    {0, 0, 0.6, 0, -0.02, 0},     {0, 0, -5.32, -0.1, 0.034, 0},
      {-0.7231, 0.5128, -7.7208, -0.1451, 0.0493, -0.0051}, {1, 0, 5.32, 0.1, -0.034, 0}, {0, 0, 0.4, 0, 0.02, 0},
  };
  expect_matrix(res, 6, 6, (double*)expected);
  a32_matrix_free(res);
}

// Regression: a fully zero matrix produced zero-length VLAs.
TEST(Matrix, PseudoInverseZero) {
  double values[2][3] = {{0, 0, 0}, {0, 0, 0}};
  a32_matrix_t mat = a32_matrix_use((double*)values, 2, 3);

  // a zero matrix is its own pseudo inverse, which is not a failure
  bool ok = false;
  a32_matrix_t res = a32_matrix_pseudo_inverse(mat, &ok);
  ASSERT_TRUE(ok);
  double expected[3][2] = {{0, 0}, {0, 0}, {0, 0}};
  expect_matrix(res, 3, 2, (double*)expected);
  a32_matrix_free(res);
}

TEST(Matrix, InvertReportsSuccess) {
  double values[3][3] = {{2, 0, 1}, {1, 3, 2}, {1, 1, 4}};
  a32_matrix_t mat = a32_matrix_use((double*)values, 3, 3);
  bool ok = false;
  a32_matrix_t inv = a32_matrix_invert(mat, &ok);
  ASSERT_TRUE(ok);
  a32_matrix_free(inv);
}

// A singular matrix has no inverse and must be reported rather than yielding the huge
// values produced by the substituted pivot.
TEST(Matrix, InvertSingular) {
  // the first row is 3x the third minus the second, so the determinant is zero
  double values[3][3] = {{2, 0, 1}, {1, 3, 2}, {1, 1, 1}};
  a32_matrix_t mat = a32_matrix_use((double*)values, 3, 3);

  bool ok = true;
  a32_matrix_t inv = a32_matrix_invert(mat, &ok);
  ASSERT_FALSE(ok);
  double zero[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
  expect_matrix(inv, 3, 3, (double*)zero);
  a32_matrix_free(inv);

  // a NULL status must be accepted
  a32_matrix_t again = a32_matrix_invert(mat, nullptr);
  a32_matrix_free(again);
}

TEST(Matrix, InvertZeroRow) {
  double values[2][2] = {{1, 2}, {0, 0}};
  a32_matrix_t mat = a32_matrix_use((double*)values, 2, 2);
  bool ok = true;
  a32_matrix_t inv = a32_matrix_invert(mat, &ok);
  ASSERT_FALSE(ok);
  a32_matrix_free(inv);
}

// A rank deficient matrix cannot be pseudo inverted by the normal equations used here.
TEST(Matrix, PseudoInverseRankDeficient) {
  // the second row is twice the first
  double values[2][2] = {{1, 2}, {2, 4}};
  a32_matrix_t mat = a32_matrix_use((double*)values, 2, 2);

  bool ok = true;
  a32_matrix_t res = a32_matrix_pseudo_inverse(mat, &ok);
  ASSERT_FALSE(ok);
  a32_matrix_free(res);
}

TEST(Matrix, PseudoInverseReportsSuccess) {
  double values[2][3] = {{0.1, 0.2, 0.1}, {0.3, 0.4, 0.1}};
  a32_matrix_t mat = a32_matrix_use((double*)values, 2, 3);
  bool ok = false;
  a32_matrix_t res = a32_matrix_pseudo_inverse(mat, &ok);
  ASSERT_TRUE(ok);
  a32_matrix_free(res);
}
