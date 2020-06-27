#ifndef HTML_GPIO_STORAGE_H
#define HTML_GPIO_STORAGE_H

#include "html_gpio_include.h"
#include "esp_spiffs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STORAGE_DEFAULT_CONFIG() {        \
    .base_path = "/spiffs",               \
    .partition_label = NULL,              \
    .max_files = CONFIG_SPIFFS_MAX_FILES, \
    .format_if_mount_failed = false,      \
}

static const char *TAG = "html_gpio_storage";
typedef enum {
  STORAGE_APPEND      = 'a',
  STORAGE_CLOSE       = 'c',
  STORAGE_READ_WRITE  = 'o',
  STORAGE_READ        = 'r',
  STORAGE_SAVE        = 's',
  STORAGE_WRITE       = 'w',
} storage_access_mode_t;

esp_err_t setup_storage(esp_vfs_spiffs_conf_t *spiffs_config);
esp_err_t storage_access(FILE *file_src, const char *file_path, storage_access_mode_t access_mode);

#ifdef __cplusplus
}
#endif

#endif // HTML_GPIO_STORAGE_H