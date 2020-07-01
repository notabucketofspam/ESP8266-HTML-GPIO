#ifndef HTML_GPIO_CORE_H
#define HTML_GPIO_CORE_H

#include "html_gpio_include.h"
#include "html_gpio_storage.h"
#include "html_gpio_network.h"
#include "html_gpio_server.h"
#include "html_gpio_control.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Set all configs to default values */
#define CORE_DEFAULT_CONFIG() {               \
  .storage_config = STORAGE_DEFAULT_CONFIG(),  \
  .control_config = CONTROL_DEFAULT_CONFIG(), \
  .network_config = NETWORK_DEFAULT_CONFIG(),     \
  .server_config = SERVER_DEFAULT_CONFIG(),     \
}

static const char *TAG = "html_gpio_core";
/* Collection of configs, one for for each setup */
typedef struct {
  storage_config_t storage_config;  // Config for SPIFFS storage
  control_config_t control_config;  // Config for GPIO control
  network_config_t network_config;  // Config for WiFi network
  server_config_t server_config;    // Config for HTTPD server
} core_config_t;

/* Initialize all setups */
esp_err_t setup_core(const core_config_t *core_config);
/* URI handler for GPIO control request from client */
static esp_err_t gpio_get_handler(httpd_req_t *req);

#ifdef __cplusplus
}
#endif

#endif // HTML_GPIO_CORE_H