#include <stdio.h>
#include <art32/matrix.h>

int main(void) {
  /* matrix tests */

  // test matrix 2x3
  printf("matrix: test 2x2\n");
  double v2x2[2][2] = {
      {0.1, 0.2},
      {0.3, 0.4},
  };
  a32_matrix_t mat = a32_matrix_use((double*)v2x2, 2, 2);
  a32_matrix_print(mat);
  printf("matrix: ---\n");
  a32_matrix_t pinv = a32_matrix_pseudo_inverse(mat);
  a32_matrix_print(pinv);
  a32_matrix_free(pinv);

  // test matrix 2x3
  printf("matrix: test 2x3\n");
  double v2x3[2][3] = {
      {0.1, 0.2, 0.1},
      {0.3, 0.4, 0.1},
  };
  mat = a32_matrix_use((double*)v2x3, 2, 3);
  a32_matrix_print(mat);
  printf("matrix: ---\n");
  pinv = a32_matrix_pseudo_inverse(mat);
  a32_matrix_print(pinv);
  a32_matrix_free(pinv);

  // test matrix 3x3
  printf("matrix: test 3x3\n");
  double v3x3[3][3] = {
      {0.1, 0.2, 0.1},
      {0.3, 0.4, 0.1},
      {0.1, 0.2, 0.3},
  };
  mat = a32_matrix_use((double*)v3x3, 3, 3);
  a32_matrix_print(mat);
  printf("matrix: ---\n");
  pinv = a32_matrix_pseudo_inverse(mat);
  a32_matrix_print(pinv);
  a32_matrix_free(pinv);

  // test matrix 3x5
  printf("matrix: test 3x5\n");
  double v3x5z[3][5] = {
      {0.0, 0.1, 0.2, 0.0, 0.1},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.3, 0.4, 0.0, 0.1},
  };
  mat = a32_matrix_use((double*)v3x5z, 3, 5);
  a32_matrix_print(mat);
  printf("matrix: ---\n");
  pinv = a32_matrix_pseudo_inverse(mat);
  a32_matrix_print(pinv);
  a32_matrix_free(pinv);

  // test matrix 5x3
  printf("matrix: test 5x3\n");
  double v5x3[5][3] = {
      {0.0, 0.0, 0.0}, {0.1, 0.0, 0.3}, {0.2, 0.0, 0.4}, {0.0, 0.0, 0.0}, {0.1, 0.0, 0.1},
  };
  mat = a32_matrix_use((double*)v5x3, 5, 3);
  a32_matrix_print(mat);
  printf("matrix: ---\n");
  pinv = a32_matrix_pseudo_inverse(mat);
  a32_matrix_print(pinv);
  a32_matrix_free(pinv);

  // test matrix 6x6
  printf("matrix: test 6x6\n");
  double v6x6[6][6] = {
      { +0.000, +0.000, +1.000, +0.000, +1.000, +0.000 },
      { +1.000, +0.000, +0.000, +1.000, +0.000, +0.000 },
      { +0.000, +1.000, +0.000, +0.000, +0.000, +1.000 },
      { -0.000, -60.000, -10.000, +0.000, +0.000, -43.000 },
      { +0.000, -20.000, +0.000, +0.00,0 +0.000, +30.000 },
      { +100.000, +0.000, +142.000, -95.000, -141.000, +0.000 },
    };
  mat = a32_matrix_use((double*)v6x6, 6, 6);
  a32_matrix_print(mat);
  printf("matrix: ---\n");
  pinv = a32_matrix_pseudo_inverse(mat);
  a32_matrix_print(pinv);
  a32_matrix_free(pinv);
}
