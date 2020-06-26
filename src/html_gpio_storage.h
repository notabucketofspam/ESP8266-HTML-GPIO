#ifndef HTML_GPIO_STORAGE_H
#define HTML_GPIO_STORAGE_H

#include "html_gpio_include.h"
#include "esp_spiffs.h"

#ifdef __cplusplus
extern "C" {
#endif

static const char *TAG = "html_gpio_storage";

esp_err_t setup_storage(void);

#ifdef __cplusplus
}
#endif

#endif // HTML_GPIO_STORAGE_H