#include <art32/smooth.h>
#include <art32/strconv.h>
#include <esp_log.h>

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
}
