#include <art32/smooth.h>
#include <art32/strconv.h>
#include <esp_log.h>
#include <art32/motion.h>

void app_main() {
  // test smooth
  a32_smooth_t* smooth = a32_smooth_new(10);
  a32_smooth_update(smooth, 5);
  a32_smooth_update(smooth, 10);
  ESP_LOGI("smooth", "%f (10.0000)", a32_smooth_update(smooth, 15));
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
  a32_motion_t motion = {0};
  motion.max_velocity = 10;
  motion.max_acceleration = 1;
  a32_motion_update(&motion, 100, 0);
  ESP_LOGI("motion", "motion: %s (0)", a32_d2str(motion.position));
  a32_motion_update(&motion, 100, 1);
  ESP_LOGI("motion", "motion: %s (0)", a32_d2str(motion.position));
  a32_motion_update(&motion, 100, 2);
  ESP_LOGI("motion", "motion: %s (1)", a32_d2str(motion.position));
  a32_motion_update(&motion, 100, 3);
  ESP_LOGI("motion", "motion: %s (3)", a32_d2str(motion.position));
  a32_motion_update(&motion, 100, 4);
  ESP_LOGI("motion", "motion: %s (6)", a32_d2str(motion.position));
}
