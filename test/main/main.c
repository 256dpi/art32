#include <esp_log.h>

#include <art32/matrix.h>
#include <art32/motion.h>
#include <art32/smooth.h>
#include <art32/strconv.h>

void app_main() {
  // test smooth
  a32_smooth_t* smooth = a32_smooth_new(10);
  ESP_LOGI("smooth", "%f %f %f (5.0000, 5.0000, 5.0000)", a32_smooth_update(smooth, 5), smooth->min, smooth->max);
  ESP_LOGI("smooth", "%f %f %f (7.5000, 5.0000, 10.0000)", a32_smooth_update(smooth, 10), smooth->min, smooth->max);
  ESP_LOGI("smooth", "%f %f %f (10.0000, 5.0000, 15.0000)", a32_smooth_update(smooth, 15), smooth->min, smooth->max);
  a32_smooth_free(smooth);

  // test strconv
  ESP_LOGI("strconv", "d2str: %s (10.0000)", a32_d2str(10.0));
  ESP_LOGI("strconv", "f2str: %s (10.0000)", a32_f2str(10.0));
  ESP_LOGI("strconv", "l2str: %s (10)", a32_l2str(10));
  ESP_LOGI("strconv", "i2str: %s (10)", a32_i2str(10));
  ESP_LOGI("strconv", "str2d: %.4f (10.0000)", a32_str2d("10.0"));
  ESP_LOGI("strconv", "str2f: %.4f (10.0000)", a32_str2f("10.0"));
  ESP_LOGI("strconv", "str2l: %ld (10)", a32_str2l("10"));
  ESP_LOGI("strconv", "str2i: %d (10)", a32_str2i("10"));

  // test motion
  a32_motion_t motion = {.max_velocity = 10, .max_acceleration = 1};
  a32_motion_update(&motion, 100, 1);
  ESP_LOGI("motion", "motion: %s (1)", a32_d2str(motion.position));
  a32_motion_update(&motion, 100, 1);
  ESP_LOGI("motion", "motion: %s (3)", a32_d2str(motion.position));
  a32_motion_update(&motion, 100, 1);
  ESP_LOGI("motion", "motion: %s (6)", a32_d2str(motion.position));
  a32_motion_update(&motion, 100, 1);
  ESP_LOGI("motion", "motion: %s (10)", a32_d2str(motion.position));
  a32_motion_update(&motion, 100, 1);
  ESP_LOGI("motion", "motion: %s (15)", a32_d2str(motion.position));

  // test matrix 2x3
  ESP_LOGI("matrix", "test 2x2");
  double v2x2[2][2] = {
      {0.1, 0.2},
      {0.3, 0.4},
  };
  a32_matrix_t mat = a32_matrix_use((double*)v2x2, 2, 2);
  a32_matrix_print(mat);
  ESP_LOGI("matrix", "---");
  a32_matrix_t pinv = a32_matrix_pseudo_inverse(mat);
  a32_matrix_print(pinv);
  a32_matrix_free(pinv);

  // test matrix 2x3
  ESP_LOGI("matrix", "test 2x3");
  double v2x3[2][3] = {
      {0.1, 0.2, 0.1},
      {0.3, 0.4, 0.1},
  };
  mat = a32_matrix_use((double*)v2x3, 2, 3);
  a32_matrix_print(mat);
  ESP_LOGI("matrix", "---");
  pinv = a32_matrix_pseudo_inverse(mat);
  a32_matrix_print(pinv);
  a32_matrix_free(pinv);

  // test matrix 3x3
  ESP_LOGI("matrix", "test 3x3");
  double v3x3[3][3] = {
      {0.1, 0.2, 0.1},
      {0.3, 0.4, 0.1},
      {0.1, 0.2, 0.3},
  };
  mat = a32_matrix_use((double*)v3x3, 3, 3);
  a32_matrix_print(mat);
  ESP_LOGI("matrix", "---");
  pinv = a32_matrix_pseudo_inverse(mat);
  a32_matrix_print(pinv);
  a32_matrix_free(pinv);

  // test matrix 3x5z
  ESP_LOGI("matrix", "test 3x5z");
  double v3x5z[3][5] = {
      {0.0, 0.1, 0.2, 0.0, 0.1},
      {0.0, 0.0, 0.0, 0.0, 0.0},
      {0.0, 0.3, 0.4, 0.0, 0.1},
  };
  mat = a32_matrix_use((double*)v3x5z, 3, 5);
  a32_matrix_print(mat);
  ESP_LOGI("matrix", "---");
  pinv = a32_matrix_pseudo_inverse(mat);
  a32_matrix_print(pinv);
  a32_matrix_free(pinv);

  // test matrix 5x3z
  ESP_LOGI("matrix", "test 5x3z");
  double v5x3z[5][3] = {
      {0.0, 0.0, 0.0}, {0.1, 0.0, 0.3}, {0.2, 0.0, 0.4}, {0.0, 0.0, 0.0}, {0.1, 0.0, 0.1},
  };
  mat = a32_matrix_use((double*)v5x3z, 5, 3);
  a32_matrix_print(mat);
  ESP_LOGI("matrix", "---");
  pinv = a32_matrix_pseudo_inverse(mat);
  a32_matrix_print(pinv);
  a32_matrix_free(pinv);
}
