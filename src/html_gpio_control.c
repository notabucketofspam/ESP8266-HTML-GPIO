#include "html_gpio_control.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t setup_control(const control_config_t *control_config) {
  ESP_LOGI(TAG, "Setup control");

  ESP_LOGI(TAG, "Control OK");
  return ESP_OK;
}

#ifdef __cplusplus
}
#endif
