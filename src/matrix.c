// Original implementation by Max Kriegleder.

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <art32/matrix.h>

#define TINY 1.0e-20f;

static bool a32_matrix_lu_decompose(a32_matrix_t mat, a32_vector_t index) {
  size_t i_max = 0;
  double big, dum, sum, temp, d;

  a32_vector_t vv = a32_vector_new(mat.cols);
  d = 1.0f;

  for (size_t i = 0; i < mat.rows; i++) {
    big = 0.0f;

    for (size_t j = 0; j < mat.cols; j++) {
      if ((temp = fabs(mat.data[i][j])) > big) {
        big = temp;
      }
    }

    if (big == 0.0) {
      a32_vector_free(vv);
      return false;
    }

    vv.data[i] = 1.0f / big;
  }

  for (size_t j = 0; j < mat.rows; j++) {
    for (size_t i = 0; i < j; i++) {
      sum = mat.data[i][j];

      for (size_t k = 0; k < i; k++) {
        sum -= mat.data[i][k] * mat.data[k][j];
      }

      mat.data[i][j] = sum;
    }

    big = 0.0f;

    for (size_t i = j; i < mat.rows; i++) {
      sum = mat.data[i][j];

      for (size_t k = 0; k < j; k++) {
        sum -= mat.data[i][k] * mat.data[k][j];
      }

      mat.data[i][j] = sum;

      if ((dum = (vv.data[i] * fabs(sum))) >= big) {
        big = dum;
        i_max = i;
      }
    }

    if (j != i_max) {
      for (size_t k = 0; k < mat.rows; k++) {
        dum = mat.data[i_max][k];
        mat.data[i_max][k] = mat.data[j][k];
        mat.data[j][k] = dum;
      }

      d = -(d);
      vv.data[i_max] = vv.data[j];
    }

    index.data[j] = i_max;

    if (mat.data[j][j] == 0.0f) {
      mat.data[j][j] = TINY
    }

    if (j != (mat.rows - 1)) {
      dum = 1.0f / (mat.data[j][j]);

      for (size_t i = j + 1; i < mat.rows; i++) {
        mat.data[i][j] *= dum;
      }
    }
  }

  a32_vector_free(vv);

  return true;
}

static void a32_matrix_lu_back_substitute(a32_matrix_t mat, a32_vector_t index, a32_vector_t b) {
  int8_t ip, ii = -1;

  double sum;

  for (size_t i = 0; i < mat.rows; i++) {
    ip = index.data[i];
    sum = b.data[ip];
    b.data[ip] = b.data[i];

    if (ii != -1)
      for (size_t j = ii; j <= (i - 1); j++) sum -= mat.data[i][j] * b.data[j];
    else if (sum)
      ii = i;

    b.data[i] = sum;
  }

  for (int8_t i = (mat.rows - 1); i >= 0; i--) {
    sum = b.data[i];
    for (size_t j = (i + 1); j < mat.rows; j++) sum -= mat.data[i][j] * b.data[j];

    b.data[i] = sum / mat.data[i][i];
  }
}

a32_matrix_t a32_matrix_new(size_t rows, size_t cols) {
  // prepare matrix
  a32_matrix_t matrix = {
      .rows = rows,
      .cols = cols,
      .data = calloc(sizeof(double *), rows),
  };

  // allocate rows
  for (size_t i = 0; i < rows; i++) {
    matrix.data[i] = calloc(sizeof(double), cols);
  }

  return matrix;
}

void a32_matrix_free(a32_matrix_t mat) {
  // free cols
  for (size_t i = 0; i < mat.rows; i++) {
    free(mat.data[i]);
  }

  // free rows
  free(mat.data);
}

a32_matrix_t a32_matrix_copy(a32_matrix_t mat) {
  // allocate
  a32_matrix_t out = a32_matrix_new(mat.rows, mat.cols);

  // transpose data
  for (size_t i = 0; i < mat.rows; i++) {
    for (size_t j = 0; j < mat.cols; j++) {
      out.data[i][j] = mat.data[i][j];
    }
  }

  return out;
}

void a32_matrix_set_row(a32_matrix_t mat, size_t row, a32_vector_t vec) {
  for (size_t i = 0; i < vec.len; i++) {
    mat.data[row][i] = vec.data[i];
  }
}

void a32_matrix_set_col(a32_matrix_t mat, size_t col, a32_vector_t vec) {
  for (size_t i = 0; i < vec.len; i++) {
    mat.data[i][col] = vec.data[i];
  }
}

a32_matrix_t a32_matrix_transpose(a32_matrix_t mat) {
  // allocate
  a32_matrix_t out = a32_matrix_new(mat.cols, mat.rows);

  // transpose data
  for (size_t i = 0; i < mat.rows; i++) {
    for (size_t j = 0; j < mat.cols; j++) {
      out.data[j][i] = mat.data[i][j];
    }
  }

  return out;
}

a32_matrix_t a32_matrix_multiply(a32_matrix_t mat1, a32_matrix_t mat2) {
  // allocate
  a32_matrix_t out = a32_matrix_new(mat1.rows, mat2.cols);

  // product data
  for (size_t i = 0; i < mat1.rows; i++) {
    for (size_t j = 0; j < mat2.cols; j++) {
      double sum = 0;
      for (size_t k = 0; k < mat1.cols; k++) {
        sum += mat1.data[i][k] * mat2.data[k][j];
      }
      out.data[i][j] = sum;
    }
  }

  return out;
}

a32_matrix_t a32_matrix_invert(a32_matrix_t mat) {
  // copy
  a32_matrix_t out = a32_matrix_copy(mat);
  a32_matrix_t temp = a32_matrix_copy(mat);

  // decompose
  a32_vector_t index = a32_vector_new(mat.rows);
  a32_matrix_lu_decompose(temp, index);

  a32_vector_t col = a32_vector_new(mat.rows);

  for (size_t j = 0; j < mat.rows; j++) {
    for (size_t i = 0; i < mat.rows; i++) {
      col.data[i] = 0.0f;
    }

    col.data[j] = 1.0f;

    a32_matrix_lu_back_substitute(temp, index, col);

    for (size_t i = 0; i < mat.rows; i++) {
      out.data[i][j] = col.data[i];
    }
  }

  // free intermediaries
  a32_vector_free(col);
  a32_vector_free(index);
  a32_matrix_free(temp);

  return out;
}

a32_matrix_t a32_matrix_pseudo_inverse(a32_matrix_t mat) {
  // prepare maps for valid rows and columns
  size_t *row_map = calloc(sizeof(size_t), mat.rows);
  size_t *col_map = calloc(sizeof(size_t), mat.cols);

  // set row map and collect valid rows
  size_t valid_rows = 0;
  for (size_t r = 0; r < mat.rows; r++) {
    row_map[r] = SIZE_MAX;
    for (size_t c = 0; c < mat.cols; c++) {
      if (mat.data[r][c] != 0) {
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
  for (size_t c = 0; c < mat.cols; c++) {
    col_map[c] = SIZE_MAX;
    for (size_t r = 0; r < mat.rows; r++) {
      if (mat.data[r][c] != 0) {
        col_map[c] = 0;
      }
    }
    if (col_map[c] == 0) {
      col_map[c] = valid_cols;
      valid_cols++;
    }
  }

  // prepare temporary matrix (without zero row and columns)
  a32_matrix_t temp = a32_matrix_new(valid_rows, valid_cols);

  // construct temporary matrix rows
  for (size_t mr = 0; mr < mat.rows; mr++) {
    // skip if zero
    if (row_map[mr] == SIZE_MAX) {
      continue;
    }

    // construct temporary matrix columns
    for (size_t mc = 0; mc < mat.cols; mc++) {
      // skip if zero
      if (col_map[mc] == SIZE_MAX) {
        continue;
      }

      // copy value
      size_t tr = row_map[mr];
      size_t tc = col_map[mc];
      temp.data[tr][tc] = mat.data[mr][mc];
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
  a32_matrix_t final = a32_matrix_new(mat.cols, mat.rows);

  // construct final matrix rows
  for (size_t fr = 0; fr < final.rows; fr++) {
    // add zero row if column was zero
    if (col_map[fr] == SIZE_MAX) {
      for (size_t mc = 0; mc < mat.cols; mc++) {
        final.data[fr][mc] = 0;
      }
      continue;
    }

    // construct temporary matrix columns
    for (size_t fc = 0; fc < final.cols; fc++) {
      // add zero column if row was zero
      if (row_map[fc] == SIZE_MAX) {
        final.data[fr][fc] = 0;
        continue;
      }

      // copy value
      size_t or = col_map[fr];
      size_t oc = row_map[fc];
      final.data[fr][fc] = output.data[or][oc];
    }
  }

  // free matrices
  a32_matrix_free(output);
  a32_matrix_free(inverse);
  a32_matrix_free(product);
  a32_matrix_free(transpose);
  a32_matrix_free(temp);

  // free maps
  free(col_map);
  free(row_map);

  return final;
}

void a32_matrix_print(a32_matrix_t mat) {
  for (size_t r = 0; r < mat.rows; r++) {
    printf("[");

    for (size_t c = 0; c < mat.cols; c++) {
      printf("%+.3f%s", mat.data[r][c], c + 1 < mat.cols ? " " : "");
    }

    printf("]\n");
  }
}
