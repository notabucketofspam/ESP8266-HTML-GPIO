#ifndef HTML_GPIO_CORE_H
#define HTML_GPIO_CORE_H

#include "html_gpio_include.h"
#include "html_gpio_storage.h"
#include "html_gpio_network.h"
#include "html_gpio_server.h"

#ifdef __cplusplus
extern "C" {
#endif

static const char *TAG = "html_gpio_core";

esp_err_t setup_core(void);

#ifdef __cplusplus
}
#endif

#endif // HTML_GPIO_CORE_H