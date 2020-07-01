#ifndef HTML_GPIO_STORAGE_H
#define HTML_GPIO_STORAGE_H

#include "html_gpio_include.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Set storage config to default values */
#define STORAGE_DEFAULT_CONFIG() {        \
    .base_path = "/spiffs",               \
    .partition_label = NULL,              \
    .max_files = CONFIG_SPIFFS_MAX_FILES, \
    .format_if_mount_failed = false,      \
}

static const char *TAG = "html_gpio_storage";
/* Operation to perform on storage */
typedef enum {
  STORAGE_APPEND      = 'a',
  STORAGE_CLOSE       = 'c',
  STORAGE_READ_WRITE  = 'o',
  STORAGE_READ        = 'r',
  STORAGE_SAVE        = 's',
  STORAGE_WRITE       = 'w',
} storage_access_mode_t;
/* Storage filesystem settings */
typedef esp_vfs_spiffs_conf_t storage_config_t;

/* Initialize SPIFFS storage */
esp_err_t setup_storage(const storage_config_t *storage_config);
/* Control files in SPIFFS */
esp_err_t storage_access(FILE *file_src, const char *file_path, storage_access_mode_t access_mode);

#ifdef __cplusplus
}
#endif

#endif // HTML_GPIO_STORAGE_H