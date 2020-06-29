#include "html_gpio_core.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t setup_core(const core_config_t *core_config) {
  ESP_LOGI(TAG, "Setup core");
  ESP_ERROR_CHECK(esp_event_loop_create_default());;
  ESP_ERROR_CHECK(setup_storage(&core_config->storage_config));
  ESP_ERROR_CHECK(setup_control(&core_config->control_config));
  ESP_ERROR_CHECK(storage_access(s_gpio_state, "/spiffs/gpio_state", STORAGE_READ_WRITE));
  ESP_ERROR_CHECK(setup_network(&core_config->network_config));
  ESP_ERROR_CHECK(storage_access(s_index_html, "/spiffs/index.html", STORAGE_READ));
  ESP_ERROR_CHECK(setup_server(&core_config->server_config));
  ESP_LOGI(TAG, "Core OK");
  return ESP_OK;
}

#ifdef __cplusplus
}
#endif