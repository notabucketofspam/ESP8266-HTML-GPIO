#include "html_gpio_storage.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t setup_storage(void) {
  ESP_LOGI(TAG, "Setup storage");
  esp_vfs_spiffs_conf_t spiffs_config = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 16,
    .format_if_mount_failed = false,
	};
	ESP_ERROR_CHECK(esp_vfs_spiffs_register(&spiffs_config));
  size_t total_bytes, used_bytes;
  ESP_ERROR_CHECK(esp_spiffs_info(NULL, &total_bytes, &used_bytes));
  ESP_LOGD(TAG,"SPIFFS: total bytes: %llu used bytes: %llu", total_bytes, used_bytes);
  ESP_LOGI(TAG, "Storage OK");
  return ESP_OK;
}

#ifdef __cplusplus
}
#endif