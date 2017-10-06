#include <art32/smooth.h>
#include <art32/strconv.h>
#include <esp_log.h>

void app_main() {
  // test smooth
  a32_smooth_t* smooth = a32_smooth_new(10);
  a32_smooth_update(smooth, 5);
  a32_smooth_update(smooth, 10);
  ESP_LOGI("smooth", "%f (10.0)", a32_smooth_update(smooth, 15));

  // test strconv
  ESP_LOGI("strconv", "dostr: %s (10.0)", a32_dtostr(10.0));
}
