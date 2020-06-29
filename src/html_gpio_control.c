#include "html_gpio_control.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t setup_control(const control_config_t *control_config) {
  ESP_LOGI(TAG, "Setup control");
  unsigned char config_index;
  for (config_index = 0; config_index < GPIO_PIN_COUNT; ++config_index) {
    ESP_ERROR_CHECK(gpio_config(&control_config->control_list[config_index]));
  }
  if (control_config->persistent_pin_state) {
    
  }
  ESP_LOGI(TAG, "Control OK");
  return ESP_OK;
}

#ifdef __cplusplus
}
#endif
