#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <art32/matrix.h>
#include <art32/vector.h>

#define TINY 1.0e-20;

static bool lu_decompose(a32_matrix_t mat, a32_vector_t index) {
  size_t i_max = 0;
  double big, dum, sum, temp, d;

  A32_VECTOR_MAKE(vv, mat.cols);
  d = 1.0;

  for (size_t i = 0; i < mat.rows; i++) {
    big = 0.0;

    for (size_t j = 0; j < mat.cols; j++) {
      if ((temp = fabs(A32_MAT(mat, i, j))) > big) {
        big = temp;
      }
    }

    if (big == 0.0) {
      return false;
    }

    vv.values[i] = 1.0 / big;
  }

  for (size_t j = 0; j < mat.rows; j++) {
    for (size_t i = 0; i < j; i++) {
      sum = A32_MAT(mat, i, j);

      for (size_t k = 0; k < i; k++) {
        sum -= A32_MAT(mat, i, k) * A32_MAT(mat, k, j);
      }

      A32_MAT(mat, i, j) = sum;
    }

    big = 0.0;

    for (size_t i = j; i < mat.rows; i++) {
      sum = A32_MAT(mat, i, j);

      for (size_t k = 0; k < j; k++) {
        sum -= A32_MAT(mat, i, k) * A32_MAT(mat, k, j);
      }

      A32_MAT(mat, i, j) = sum;

      if ((dum = (vv.values[i] * fabs(sum))) >= big) {
        big = dum;
        i_max = i;
      }
    }

    if (j != i_max) {
      for (size_t k = 0; k < mat.rows; k++) {
        dum = A32_MAT(mat, i_max, k);
        A32_MAT(mat, i_max, k) = A32_MAT(mat, j, k);
        A32_MAT(mat, j, k) = dum;
      }

      d = -(d);
      vv.values[i_max] = vv.values[j];
    }

    index.values[j] = i_max;

    if (A32_MAT(mat, j, j) == 0.0) {
      A32_MAT(mat, j, j) = TINY
    }

    if (j != (mat.rows - 1)) {
      dum = 1.0 / A32_MAT(mat, j, j);

      for (size_t i = j + 1; i < mat.rows; i++) {
        A32_MAT(mat, i, j) *= dum;
      }
    }
  }

  return true;
}

static void lu_back_substitute(a32_matrix_t mat, a32_vector_t index, a32_vector_t b) {
  int8_t ip, ii = -1;

  double sum;

  for (size_t i = 0; i < mat.rows; i++) {
    ip = index.values[i];
    sum = b.values[ip];
    b.values[ip] = b.values[i];

    if (ii != -1) {
      for (size_t j = ii; j <= (i - 1); j++) {
        sum -= A32_MAT(mat, i, j) * b.values[j];
      }
    } else if (sum) {
      ii = i;
    }

    b.values[i] = sum;
  }

  for (int8_t i = (mat.rows - 1); i >= 0; i--) {
    sum = b.values[i];
    for (size_t j = (i + 1); j < mat.rows; j++) {
      sum -= A32_MAT(mat, i, j) * b.values[j];
    }

    b.values[i] = sum / A32_MAT(mat, i, i);
  }
}

a32_matrix_t a32_matrix_new(size_t rows, size_t cols) {
  // allocate values
  double *values = calloc(sizeof(double), rows * cols);

  // prepare matrix
  a32_matrix_t mat = {
      .rows = rows,
      .cols = cols,
      .values = values,
  };

  return mat;
}

void a32_matrix_free(a32_matrix_t mat) {
  // free values
  free(mat.values);
}

a32_matrix_t a32_matrix_use(const double *values, size_t rows, size_t cols) {
  // prepare matrix
  a32_matrix_t mat = {
      .rows = rows,
      .cols = cols,
      .values = (double *)values,
  };

  return mat;
}

void a32_matrix_copy(a32_matrix_t dst, a32_matrix_t src) {
  // assert shape
  assert(dst.rows == src.rows);
  assert(dst.cols == src.cols);

  // copy values
  memcpy(dst.values, src.values, sizeof(double) * src.rows * src.cols);
}

a32_vector_t a32_matrix_get_row(a32_matrix_t mat, size_t row) {
  // assert shape
  assert(row < mat.rows);

  // allocate
  a32_vector_t vec = a32_vector_new(mat.cols);

  // copy values from matrix
  for (size_t c = 0; c < vec.length; c++) {
    vec.values[c] = A32_MAT(mat, row, c);
  }

  return vec;
}

a32_vector_t a32_matrix_get_col(a32_matrix_t mat, size_t col) {
  // assert shape
  assert(col < mat.cols);

  // allocate
  a32_vector_t vec = a32_vector_new(mat.rows);

  // copy values from matrix
  for (size_t r = 0; r < vec.length; r++) {
    vec.values[r] = A32_MAT(mat, r, col);
  }

  return vec;
}

void a32_matrix_set_row(a32_matrix_t mat, size_t row, a32_vector_t vec) {
  // assert shape
  assert(row < mat.rows);
  assert(mat.cols == vec.length);

  // copy values from vector
  for (size_t c = 0; c < vec.length; c++) {
    A32_MAT(mat, row, c) = vec.values[c];
  }
}

void a32_matrix_set_col(a32_matrix_t mat, size_t col, a32_vector_t vec) {
  // assert shape
  assert(col < mat.cols);
  assert(mat.rows == vec.length);

  // copy values from vector
  for (size_t r = 0; r < vec.length; r++) {
    A32_MAT(mat, r, col) = vec.values[r];
  }
}

a32_matrix_t a32_matrix_transpose(a32_matrix_t mat) {
  // allocate result
  a32_matrix_t result = a32_matrix_new(mat.cols, mat.rows);

  // transpose
  for (size_t r = 0; r < mat.rows; r++) {
    for (size_t c = 0; c < mat.cols; c++) {
      A32_MAT(result, c, r) = A32_MAT(mat, r, c);
    }
  }

  return result;
}

void a32_matrix_add(a32_matrix_t mat, a32_matrix_t add) {
  // assert shape
  assert(mat.rows == add.rows);
  assert(mat.cols == add.cols);

  // multiply
  for (size_t r = 0; r < mat.rows; r++) {
    for (size_t c = 0; c < add.cols; c++) {
      A32_MAT(mat, r, c) += A32_MAT(add, r, c);
    }
  }
}

void a32_matrix_subtract(a32_matrix_t mat, a32_matrix_t sub) {
  // assert shape
  assert(mat.rows == sub.rows);
  assert(mat.cols == sub.cols);

  // multiply
  for (size_t r = 0; r < mat.rows; r++) {
    for (size_t c = 0; c < sub.cols; c++) {
      A32_MAT(mat, r, c) -= A32_MAT(sub, r, c);
    }
  }
}

a32_matrix_t a32_matrix_multiply(a32_matrix_t mat1, a32_matrix_t mat2) {
  // assert shape
  assert(mat1.cols == mat2.rows);

  // allocate result
  a32_matrix_t result = a32_matrix_new(mat1.rows, mat2.cols);

  // multiply
  for (size_t r = 0; r < mat1.rows; r++) {
    for (size_t c = 0; c < mat2.cols; c++) {
      double sum = 0;
      for (size_t k = 0; k < mat1.cols; k++) {
        sum += A32_MAT(mat1, r, k) * A32_MAT(mat2, k, c);
      }
      A32_MAT(result, r, c) = sum;
    }
  }

  return result;
}

void a32_matrix_multiply_scalar(a32_matrix_t mat, double scalar) {
  // multiply
  for (size_t r = 0; r < mat.rows; r++) {
    for (size_t c = 0; c < mat.cols; c++) {
      A32_MAT(mat, r, c) *= scalar;
    }
  }
}

a32_vector_t a32_vector_multiply_matrix(a32_vector_t vec, a32_matrix_t mat) {
  // assert shape
  assert(vec.length == mat.cols);

  // allocate result
  a32_vector_t result = a32_vector_new(mat.rows);

  // multiply
  for (size_t r = 0; r < mat.rows; r++) {
    double sum = 0;
    for (size_t c = 0; c < vec.length; c++) {
      sum += A32_MAT(mat, r, c) * vec.values[c];
    }
    result.values[r] = sum;
  }

  return result;
}

a32_matrix_t a32_matrix_invert(a32_matrix_t mat) {
  // assert shape
  assert(mat.rows == mat.cols);

  // copy result
  a32_matrix_t result = a32_matrix_new(mat.rows, mat.cols);
  a32_matrix_copy(result, mat);

  // copy temp
  A32_MATRIX_MAKE(temp, mat.rows, mat.cols);
  a32_matrix_copy(temp, mat);

  // decompose
  A32_VECTOR_MAKE(index, mat.rows);
  lu_decompose(temp, index);

  // prepare column
  A32_VECTOR_MAKE(col, mat.rows);

  // invert
  for (size_t j = 0; j < mat.rows; j++) {
    for (size_t i = 0; i < mat.rows; i++) {
      col.values[i] = 0.0;
    }
    col.values[j] = 1.0;
    lu_back_substitute(temp, index, col);
    for (size_t i = 0; i < mat.rows; i++) {
      A32_MAT(result, i, j) = col.values[i];
    }
  }

  return result;
}

a32_matrix_t a32_matrix_pseudo_inverse(a32_matrix_t mat) {
  // get input
  a32_matrix_t input = mat;

  // flip if tall
  bool flipped = false;
  if (input.rows > input.cols) {
    input = a32_matrix_transpose(input);
    flipped = true;
  }

  // prepare maps for valid rows and columns
  size_t row_map[input.rows];
  size_t col_map[input.cols];

  // set row map and collect valid rows
  size_t valid_rows = 0;
  for (size_t r = 0; r < input.rows; r++) {
    row_map[r] = SIZE_MAX;
    for (size_t c = 0; c < input.cols; c++) {
      if (A32_MAT(input, r, c) != 0) {
        row_map[r] = 0;
      }
    }
    if (row_map[r] == 0) {
      row_map[r] = valid_rows;
      valid_rows++;
    }
  }

  // set column map and collect valid columns
  size_t valid_cols = 0;
  for (size_t c = 0; c < input.cols; c++) {
    col_map[c] = SIZE_MAX;
    for (size_t r = 0; r < input.rows; r++) {
      if (A32_MAT(input, r, c) != 0) {
        col_map[c] = 0;
      }
    }
    if (col_map[c] == 0) {
      col_map[c] = valid_cols;
      valid_cols++;
    }
  }

  // prepare temporary matrix (without zero row and columns)
  A32_MATRIX_MAKE(temp, valid_rows, valid_cols);

  // construct temporary matrix rows
  for (size_t mr = 0; mr < input.rows; mr++) {
    // skip if zero
    if (row_map[mr] == SIZE_MAX) {
      continue;
    }

    // construct temporary matrix columns
    for (size_t mc = 0; mc < input.cols; mc++) {
      // skip if zero
      if (col_map[mc] == SIZE_MAX) {
        continue;
      }

      // copy value
      size_t tr = row_map[mr];
      size_t tc = col_map[mc];
      A32_MAT(temp, tr, tc) = A32_MAT(input, mr, mc);
    }
  }

  // transpose
  a32_matrix_t transpose = a32_matrix_transpose(temp);

  // multiply
  a32_matrix_t product = a32_matrix_multiply(temp, transpose);

  // invert
  a32_matrix_t inverse = a32_matrix_invert(product);

  // final transpose
  a32_matrix_t output = a32_matrix_multiply(transpose, inverse);

  // construct final matrix (with zero rows and columns)
  a32_matrix_t final = a32_matrix_new(input.cols, input.rows);

  // construct final matrix rows
  for (size_t fr = 0; fr < final.rows; fr++) {
    // construct temporary matrix columns
    for (size_t fc = 0; fc < final.cols; fc++) {
      // get original indexes
      size_t or = col_map[fr];
      size_t oc = row_map[fc];

      // set zero value if row or column was zero
      if (or == SIZE_MAX || oc == SIZE_MAX) {
        A32_MAT(final, fr, fc) = 0;
        continue;
      }

      // copy value
      A32_MAT(final, fr, fc) = A32_MAT(output, or, oc);
    }
  }

  // free matrices
  a32_matrix_free(output);
  a32_matrix_free(inverse);
  a32_matrix_free(product);
  a32_matrix_free(transpose);

  // get result
  a32_matrix_t result = final;

  // flip back
  if (flipped) {
    result = a32_matrix_transpose(final);
    a32_matrix_free(final);
    a32_matrix_free(input);
  }

  return result;
}

void a32_matrix_print(a32_matrix_t mat) {
  // print layout
  for (size_t r = 0; r < mat.rows; r++) {
    printf("[");
    for (size_t c = 0; c < mat.cols; c++) {
      printf("%+.3f%s", A32_MAT(mat, r, c), c + 1 < mat.cols ? " " : "");
    }
    printf("]\n");
  }
}
