#include "html_gpio_storage.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t setup_storage(const storage_config_t *storage_config) {
  ESP_LOGI(TAG, "Setup storage");
	ESP_ERROR_CHECK(esp_vfs_spiffs_register(&storage_config));
  #if CONFIG_LOG_DEFAULT_LEVEL > 3
    size_t total_bytes, used_bytes;
    ESP_ERROR_CHECK(esp_spiffs_info(NULL, &total_bytes, &used_bytes));
    ESP_LOGD(TAG,"SPIFFS: total bytes: %llu used bytes: %llu", total_bytes, used_bytes);
  #endif
  ESP_LOGI(TAG, "Storage OK");
  return ESP_OK;
}

esp_err_t storage_access(FILE *file_src, const char *file_path, storage_access_mode_t access_mode) {
  if (file_src == NULL || file_path == NULL) return ESP_FAIL;
  static char s_access_mode[2];
  memset(s_access_mode, 0x00, sizeof(s_access_mode));
  memcpy(s_access_mode, access_mode, 1);
  switch (access_mode) {
    case STORAGE_CLOSE:
      fclose(file_src);
      break;
    case STORAGE_SAVE:
      fflush(file_src);
      break;
    case STORAGE_READ_WRITE:
      file_src = fopen(file_path, "r+");
      break;
    default:
      file_src = fopen(file_path, s_access_mode);
      break;
    return ESP_OK;
  }
}

#ifdef __cplusplus
}
#endif