#include "../src/html_gpio_include.h"
#include "../src/html_gpio_storage.h"
#include "../src/html_gpio_network.h"
#include "../src/html_gpio_server.h"
#include "../src/html_gpio_control.h"

#ifdef __cplusplus
extern "C" {
#endif

void app_main(void) {
  ESP_LOGI(TAG, "app_main init");
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_vfs_spiffs_conf_t spiffs_config = STORAGE_DEFAULT_CONFIG();
  ESP_ERROR_CHECK(setup_storage(&spiffs_config));
  ESP_ERROR_CHECK(storage_access(s_index_html, "/spiffs/index.html", STORAGE_READ));
  ESP_ERROR_CHECK(storage_access(s_gpio_state, "/spiffs/gpio_state", STORAGE_READ_WRITE));
  control_config_t control_config = CONTROL_DEFAULT_CONFIG();
  ESP_ERROR_CHECK(setup_control(&control_config));
  wifi_config_t sta_config = NETWORK_DEFAULT_CONFIG();
  ESP_ERROR_CHECK(setup_network(&sta_config));
  httpd_config_t httpd_config = HTTPD_DEFAULT_CONFIG();
  ESP_ERROR_CHECK(setup_server(&httpd_config));
  ESP_LOGI(TAG, "app_main OK");
}

#ifdef __cplusplus
}
#endif
