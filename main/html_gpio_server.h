#ifndef HTML_GPIO_SERVER_H
#define HTML_GPIO_SERVER_H

#include "html_gpio_include.h"
#include "esp_http_server.h"

#ifdef __cplusplus
extern "C" {
#endif

static const char *TAG = "html_gpio_server";
static httpd_handle_t s_httpd_server;
extern httpd_uri_t uri_get;
extern httpd_uri_t uri_post;

esp_err_t setup_server(void);
esp_err_t get_handler(httpd_req_t *req);
esp_err_t post_handler(httpd_req_t *req);

#ifdef __cplusplus
}
#endif

#endif //HTML_GPIO_SERVER_H