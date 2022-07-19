#include <stdio.h>
#include <string.h>

#include "art32/filter.h"
#include "art32/matrix.h"
#include "art32/motion.h"
#include "art32/parser.h"
#include "art32/smooth.h"
#include "art32/strconv.h"

int main() {
  // test smooth
  a32_smooth_t* smooth = a32_smooth_new(10);
  printf("smooth: %f %f %f (5.0000, 5.0000, 5.0000)\n", a32_smooth_update(smooth, 5), smooth->min, smooth->max);
  printf("smooth: %f %f %f (7.5000, 5.0000, 10.0000)\n", a32_smooth_update(smooth, 10), smooth->min, smooth->max);
  printf("smooth: %f %f %f (10.0000, 5.0000, 15.0000)\n", a32_smooth_update(smooth, 15), smooth->min, smooth->max);
  a32_smooth_free(smooth);

  // test filter
  a32_filter_t* filter = a32_filter_new(5);
  printf("filter: %f (5.0000)\n", a32_filter_update(filter, 5));
  printf("filter: %f (5.0000)\n", a32_filter_update(filter, 5));
  printf("filter: %f (5.0000)\n", a32_filter_update(filter, 15));
  printf("filter: %f (7.5000)\n", a32_filter_update(filter, 10));
  printf("filter: %f (10.0000)\n", a32_filter_update(filter, 15));
  printf("filter: %f (15.0000)\n", a32_filter_update(filter, 15));
  a32_filter_free(filter);

  // test strconv
  printf("d2str: %s (10.0000)\n", a32_d2str(10.0));
  printf("f2str: %s (10.0000)\n", a32_f2str(10.0f));
  printf("l2str: %s (10)\n", a32_l2str(10));
  printf("i2str: %s (10)\n", a32_i2str(10));
  printf("str2d: %.4f (10.0000)\n", a32_str2d("10.0"));
  printf("str2f: %.4f (10.0000)\n", a32_str2f("10.0"));
  printf("str2l: %ld (10)\n", a32_str2l("10"));
  printf("str2i: %d (10)\n", a32_str2i("10"));

  // test motion
  a32_motion_t motion = {.max_velocity = 10, .max_acceleration = 1};
  a32_motion_update(&motion, 100, 1);
  printf("motion: %s (1)\n", a32_d2str(motion.position));
  a32_motion_update(&motion, 100, 1);
  printf("motion: %s (3)\n", a32_d2str(motion.position));
  a32_motion_update(&motion, 100, 1);
  printf("motion: %s (6)\n", a32_d2str(motion.position));
  a32_motion_update(&motion, 100, 1);
  printf("motion: %s (10)\n", a32_d2str(motion.position));
  a32_motion_update(&motion, 100, 1);
  printf("motion: %s (15)\n", a32_d2str(motion.position));

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

  // test matrix 3x5z
  printf("matrix: test 3x5z\n");
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

  // test matrix 5x3z
  printf("matrix: test 5x3z\n");
  double v5x3z[5][3] = {
      {0.0, 0.0, 0.0}, {0.1, 0.0, 0.3}, {0.2, 0.0, 0.4}, {0.0, 0.0, 0.0}, {0.1, 0.0, 0.1},
  };
  mat = a32_matrix_use((double*)v5x3z, 5, 3);
  a32_matrix_print(mat);
  printf("matrix: ---\n");
  pinv = a32_matrix_pseudo_inverse(mat);
  a32_matrix_print(pinv);
  a32_matrix_free(pinv);

  /* parser test */

  // log
  printf("parser:\n");

  // prepare string
  char * source = strdup("FOO 1 2.0 foo; FOO\nBAR\n");

  // prepare defs
  a32_def_t defs[] = {
          {
                  .id = 0,
                  .name = "FOO",
                  .fmt = "ifs",
          },
          {
            .id = 1,
            .name = "BAR",
            .fmt = "",
          }
  };

  // parse codes
  a32_code_t code;
  A32_PARSER_MAKE(p, source, defs);
  while (a32_parser_next(&p, &code)) {
    // handle codes
    switch (code.def->id) {
      case 0:
        printf("%s @ %ld:, %d, %f, %s\n", code.def->name, code.off, code.args[0].i, code.args[1].f, code.args[2].s);
        break;
      case 1:
        printf("%s @ %ld\n", code.def->name, code.off);
      default:
        break;
    }
  }
}
