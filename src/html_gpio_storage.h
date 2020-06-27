#ifndef HTML_GPIO_STORAGE_H
#define HTML_GPIO_STORAGE_H

#include "html_gpio_include.h"
#include "esp_spiffs.h"

#ifdef __cplusplus
extern "C" {
#endif

static const char *TAG = "html_gpio_storage";
typedef enum storage_access_mode {
  STORAGE_APPEND      = 'a',
  STORAGE_CLOSE       = 'c',
  STORAGE_READ        = 'r',
  STORAGE_READ_WRITE  = '+',
  STORAGE_SAVE        = 's',
  STORAGE_WRITE       = 'w',
} storage_access_mode_t;

esp_err_t setup_storage(void);
esp_err_t storage_access(FILE *file_src, const char *file_path, storage_access_mode_t access_mode);

#ifdef __cplusplus
}
#endif

#endif // HTML_GPIO_STORAGE_H