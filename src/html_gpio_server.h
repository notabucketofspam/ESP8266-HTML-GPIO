#ifndef HTML_GPIO_SERVER_H
#define HTML_GPIO_SERVER_H

#include "html_gpio_include.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Set server config to default values */
#define SERVER_DEFAULT_CONFIG() \
  HTTPD_DEFAULT_CONFIG()        

static const char *TAG = "html_gpio_server";
/* index.html of HTTPD server  */
static FILE *s_index_html;
/* Central HTTPD server */
static httpd_handle_t s_httpd_server;
/* HTTPD server settings */
typedef httpd_config_t server_config_t;

/* Initialize HTTPD server */
esp_err_t setup_server(const server_config_t *server_config);
/* URI handler for index.html request from client */
static esp_err_t index_html_get_handler(httpd_req_t *req);
/* URI handler for base path request from client */
static esp_err_t base_path_get_handler(httpd_req_t *req);

#ifdef __cplusplus
}
#endif

#endif //HTML_GPIO_SERVER_H