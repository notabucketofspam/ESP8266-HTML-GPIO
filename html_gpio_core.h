#ifndef HTML_GPIO_CORE_H
#define HTML_GPIO_CORE_H

#include "general_component/general_include.h"
#include "general_component/general_storage.h"
#include "general_component/general_network.h"
#include "general_component/general_server.h"
#include "general_component/general_control.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Set all configs to default values */
#define CORE_DEFAULT_CONFIG() {               \
  .storage_config = STORAGE_DEFAULT_CONFIG(), \
  .control_config = CONTROL_DEFAULT_CONFIG(), \
  .network_config = NETWORK_DEFAULT_CONFIG(), \
  .server_config = SERVER_DEFAULT_CONFIG(),   \
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
/* URI handler for pin config request from client */
static esp_err_t pins_get_handler(httpd_req_t *req);
/* URI handler for persistent pin state save request from client */
static esp_err_t save_get_handler(httpd_req_t *req);
/* URI handler for persistent pin state load request from client */
static esp_err_t load_get_handler(httpd_req_t *req);
/* URI handler for GPIO control request from client */
static esp_err_t gpio_get_handler(httpd_req_t *req);

#ifdef __cplusplus
}
#endif

#endif // HTML_GPIO_CORE_H
