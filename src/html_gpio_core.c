#include "html_gpio_core.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t setup_core(void) {
  ESP_LOGI(TAG, "Setup core");
  ESP_ERROR_CHECK(setup_storage());
  ESP_ERROR_CHECK(setup_network());
  ESP_ERROR_CHECK(setup_server());
  ESP_LOGI(TAG, "Core OK");
  return ESP_OK;
}

#ifdef __cplusplus
}
#endif